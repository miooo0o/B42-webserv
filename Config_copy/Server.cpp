#include "Server.hpp"
#include "Config.hpp" // Optional if you're using Config
#include <algorithm>  // For std::remove
#include <cstring>    // For memset
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Set up sockets and start polling loop
void Server::setupSocketsAndRun(Config* config, int numServers) {
	for (int i = 0; i < numServers; ++i) {
		int port = config[i].getPort();
		if (port <= 0) {
			std::cerr << "Invalid port for server " << i + 1 << std::endl;
			continue;
		}

		int server_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (server_fd < 0) {
			std::cerr << "Failed to create socket for server " << i + 1 << std::endl;
			continue;
		}

		int opt = 1;
		if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
			std::cerr << "Failed to set socket options" << std::endl;
			close(server_fd);
			continue;
		}

		sockaddr_in address{};
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(port);

		if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
			std::cerr << "Failed to bind socket on port " << port << std::endl;
			close(server_fd);
			continue;
		}

		if (listen(server_fd, 128) < 0) {
			std::cerr << "Failed to listen on port " << port << std::endl;
			close(server_fd);
			continue;
		}

		setNonBlocking(server_fd);
		sockets.push_back(server_fd);
		std::cout << "Server " << i + 1 << " listening on port " << port << std::endl;
	}

	runPollLoop();
}

void Server::setNonBlocking(int fd) {
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0) {
		std::cerr << "Failed to get file descriptor flags" << std::endl;
		return;
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
		std::cerr << "Failed to set non-blocking mode" << std::endl;
	}
}

void Server::closeClientConnection(int client_fd) {
	std::cout << "Closing connection on socket " << client_fd << std::endl;
	close(client_fd);
	clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), client_fd), clientSockets.end());
	clientLastActivity.erase(client_fd);
}

void Server::checkTimeouts() {
	time_t currentTime = time(nullptr);
	for (auto it = clientLastActivity.begin(); it != clientLastActivity.end();) {
		if (currentTime - it->second > TIMEOUT) {
			std::cout << "Client on socket " << it->first << " timed out." << std::endl;
			closeClientConnection(it->first);
			it = clientLastActivity.erase(it); 
		} else {
			++it;
		}
	}
}

void Server::shutdownServer() {
	std::cout << "Shutting down server..." << std::endl;
	for (size_t i = 0; i < clientSockets.size(); ++i) {
		closeClientConnection(clientSockets[i]);
	}
	for (size_t i = 0; i < sockets.size(); ++i) {
		std::cout << "Closing server socket " << sockets[i] << std::endl;
		close(sockets[i]);
	}
	clientSockets.clear();
	sockets.clear();
	clientLastActivity.clear();
	std::cout << "Server shutdown completed." << std::endl;
}

void Server::runPollLoop() {
	std::vector<pollfd> poll_fds;

	for (size_t i = 0; i < sockets.size(); ++i) {
		pollfd pfd{};
		pfd.fd = sockets[i];
		pfd.events = POLLIN;
		poll_fds.push_back(pfd);
	}

	std::cout << "Starting poll event loop..." << std::endl;

	while (true) {
		checkTimeouts();

		int event_count = poll(poll_fds.data(), poll_fds.size(), 1000); // 1 second timeout
		if (event_count < 0) {
			std::cerr << "Poll failed" << std::endl;
			break;
		}

		for (size_t i = 0; i < poll_fds.size(); ++i) {
			if (poll_fds[i].revents & POLLIN) {
				sockaddr_in client_addr;
				socklen_t client_len = sizeof(client_addr);
				int client_fd = accept(poll_fds[i].fd, (struct sockaddr*)&client_addr, &client_len);

				if (client_fd >= 0) {
					setNonBlocking(client_fd);
					clientSockets.push_back(client_fd);
					clientLastActivity[client_fd] = time(nullptr);
					std::cout << "Accepted new connection on socket " << poll_fds[i].fd << std::endl;
				}
			}
		}
	}
}

Server::~Server() {
	for (size_t i = 0; i < sockets.size(); ++i) {
		close(sockets[i]);
	}
}
