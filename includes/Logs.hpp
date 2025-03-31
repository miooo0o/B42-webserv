/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 17:08:38 by kmooney           #+#    #+#             */
/*   Updated: 2025/03/31 08:48:04 by kmooney          ###   ########.fr       */
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

#define APPEND		0
#define DELETE		1
#define CLEAR		2
#define COPY		3
#define MOVE		4
#define PRINT		5
#define WRITE		6

class Logs {

	private:
		size_t				_count;
		file_t				_file;
		size_t				_level;
		std::ostream		_stream;

	public:

		/* CONSTRUCTORS */
		Logs( void );
		Logs( size_t level, str_t entry, size_t action );
		Logs( size_t level, file_s* file, str_t entry, size_t action );
		Logs( const Logs& other );
		
		/* DESTRUCTORS */
		~Logs( void );

		/* COPY CONSTRUCTOR */
		Logs&	operator=( const Logs& other );

		/* METHODS */
		void	actionHandler();
		void	addEventToLog( size_t level, size_t action );
		void	flushStreamToFile();
};
#endif
