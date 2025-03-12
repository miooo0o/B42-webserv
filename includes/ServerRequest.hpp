/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerRequest.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 09:12:35 by kmooney           #+#    #+#             */
/*   Updated: 2025/03/11 14:22:31 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_REQUEST_HPP
# define SERVER_REQUEST_HPP

#include "./TestClasses/testUtils.hpp"
#include "../includes/Request.hpp"

#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <poll.h>
#include <sstream>
#include <stdint.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#define MAX_REQUEST_LINE 8192	// Maximum allowed request line length
#define BUFFER_SIZE 1024		// Read in chunks

class Request{};

typedef struct Client {
	Request		request;
	int			fd;							// Client socket file descriptor
	std::string	request_buffer;				// Stores partial request data
	bool 		request_line_received;		// Flag to indicate request line is fully received
	bool 		headers_received;			// Flag to indicate headers line is fully received
	bool 		body_received;				// Flag to indicate body is fully received
	Client(int socket_fd) : fd(socket_fd), request_line_received(false), headers_received(false), body_received(false) {}
};

class	ServerRequest
{
	private:
								std::map<int, Client> clients;

								/* CONSTRUCTORS */
								ServerRequest( void );
								ServerRequest( const ServerRequest& other );

								/* DESTRUCTORS */
								~ServerRequest( void );

								/* COPY ASSIGNMENT */
			ServerRequest&		operator=(const ServerRequest& other);

								/* SERVER CONNECTIONS */
			void				handle_connections(int server_fd);

								/* READ REQUESTS */
			bool				read_request_line(Client &client);
			bool				read_headers(Client &client);
			bool				read_body(Client &client);
	
	public:
		
};

#endif