/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 17:08:38 by kmooney           #+#    #+#             */
/*   Updated: 2025/03/31 12:59:57 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGS_HPP
# define LOGS_HPP

#include "core_def.hpp"
#include <iostream>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#define LOGS_ERR	0
#define LOGS_CONF	1
#define LOGS_SERV	2
#define LOGS_REQU	3
#define LOGS_RESP	4
#define LOGS_DEBUG	5

#define STRING		0
#define STREAM		1

#define APPEND		0
#define DELETE		1
#define CLEAR		2
#define PRINT		3

#define LOG_ERROR	"./logs/log_error"
#define LOG_CONFIG	"./logs/log_config"
#define LOG_SERVER	"./logs/log_server"
#define LOG_REQUEST	"./logs/log_request"
#define LOG_RESPONS	"./logs/log_response"
#define LOG_DEBUG	"./logs/log_debug"

class Logs {

	private:
		size_t				_count;
		file_t				_file;
		int					_entry_type;
		
	public:
	
		/* CONSTRUCTORS */
		Logs( void );
		Logs( log_t& log_entry, int action );
		Logs( log_t& log_entry, int action , str_t filename );
		Logs( log_t& log_entry, int action , file_t* file );
		Logs( const Logs& other );
		
		/* DESTRUCTORS */
		~Logs( void );

		/* COPY CONSTRUCTOR */
		Logs&	operator=( const Logs& other );

		/* LOG HANDLER */
		void	logHandler (log_t& entry, int action);

		/* ACTIONS */
		void	addToLog( log_t& entry );
		void	clearLog( log_t& entry );
		void	deleteLog( log_t& entry );
		void	flushStreamToFile( log_t& entry );
		void	printStdOut( log_t& entry );

		/* HELPER FUNCTIONS */
		str_t	getType_name( int entry_type );

		/* FILE OPERATIONS */
		void	openFile(str_t filename);
		void	clostFile();
};
#endif
