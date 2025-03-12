/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 09:12:35 by kmooney           #+#    #+#             */
/*   Updated: 2025/03/11 14:22:34 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerRequest.hpp"

ServerRequest::ServerRequest( void ) {
	
	classText( std::cout << GRE "Constructor called (ServerRequest)" << std::endl);
}

ServerRequest::~ServerRequest( void ) {
	
	classText( std::cout << BLU "Destructor called (ServerRequest)" << std::endl);
}

ServerRequest::ServerRequest( const ServerRequest& other ) {

	( void )other;
	classText( std::cout << GRE "Copy Constructor called (ServerRequest)" << std::endl);
}

ServerRequest& ServerRequest::operator=( const ServerRequest& other ) {
	
	classText( std::cout << GRE "Copy Assignment operator called (ServerRequest)" << std::endl);
	if ( this != &other )
	{
		return ( *this );
	}
	return ( *this );
}

void	ServerRequest::handle_connections(int server_fd)
{
	struct pollfd fds[1024];	// Support up to 1024 clients
	int nfds = 1;				// Start with 1 (server socket)
    
	fds[0].fd = server_fd;
	fds[0].events = POLLIN;

	while (true) {
		int poll_count = poll(fds, nfds, -1);  // Block until an event occurs
		if (poll_count < 0) {
			std::cerr << "poll() error\n";
			break;
		}

		for (int i = 0; i < nfds; i++) {
			if (fds[i].revents & POLLIN) {
				if (fds[i].fd == server_fd) {
					// Accept new client connection
					int client_fd = accept(server_fd, NULL, NULL);
					if (client_fd > 0) {
						fcntl(client_fd, F_SETFL, O_NONBLOCK);  // Set non-blocking mode
						clients[client_fd] = Client(client_fd);
						fds[nfds].fd = client_fd;
						fds[nfds].events = POLLIN;
						nfds++;
					}
				} else {
					// Read request line from an existing client
					Client &client = clients[fds[i].fd];
					if (read_request_line(client)) {
						std::cout << "Received request line: " << client.request_buffer << std::endl;
					}
				}
			}
		}
	}
}

bool	ServerRequest::read_request_line(Client &client)
{
	struct pollfd	pfd;

	pfd.fd = client.fd;
	pfd.events = POLLIN;  // We are only reading data

	int	poll_result = poll(&pfd, 1, 5000);  // Wait up to 5 seconds

	if (poll_result <= 0)  // Timeout or error
		return false;

	char	buffer[BUFFER_SIZE];
	ssize_t	bytes_read = recv(client.fd, buffer, BUFFER_SIZE, 0);
	
	if (bytes_read <= 0)  // Connection closed or error
		return false;

	client.request_buffer.append(buffer, bytes_read);  // Append new data

	// Look for the end of the request line
	size_t pos = client.request_buffer.find("\r\n");
	if (pos != std::string::npos) {
		client.request_line_received = true;
		return true;
	}

	// Prevent buffer overflow
	if (client.request_buffer.size() > MAX_REQUEST_LINE) {
		return false;
	}
	return false;  // Request line still incomplete
}
