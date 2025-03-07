#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "Config.hpp"  // Only if you use Config in the setupSocketsAndRun()

class Server {
	public:
		std::vector<int> sockets;
		std::vector<int> clientSockets;
		std::map<int, time_t> clientLastActivity;
		const int TIMEOUT = 300;

		void setupSocketsAndRun(Config* config, int numServers);
		void setNonBlocking(int fd);
		void closeClientConnection(int client_fd);
		void checkTimeouts();
		void shutdownServer();
		void runPollLoop();

		~Server();
};

#endif