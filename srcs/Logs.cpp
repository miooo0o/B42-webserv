/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logs.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 17:08:11 by kmooney           #+#    #+#             */
/*   Updated: 2025/04/05 17:22:49 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Logs.hpp"

Logs::Logs( void ){}
Logs::Logs( log_t& entry, int action ){logHandler(entry, action);}
Logs::Logs( log_t& entry, int action , str_t filename ): _file(filename) {logHandler(entry, action);}
Logs::Logs( log_t& entry, int action , file_t* file ): _file(*file) {logHandler(entry, action);}

Logs::~Logs( void ){}

Logs::Logs( const Logs& other ){
    *this = other;
}

Logs& Logs::operator=( const Logs& other ) {
	if ( this != &other ) {
        *this = other;
	}
	return ( *this );
}

void	Logs::logHandler( log_t& entry, int action){
	switch (action){
		case APPEND	: { addToLog( entry ); break;}
		case PRINT	: { printStdOut( entry ); break;}
		case DELETE	: { deleteLog( entry ); break;}
		case CLEAR	: { clearLog( entry ); break;}
		default		: return ;
	}
}

void	Logs::addToLog( log_t& entry ){
	str_t name = getType_name(entry. entry_type);
	
}

void	Logs::flushStreamToFile( log_t& entry ){
	(void)entry;
}

void	Logs::printStdOut( log_t& entry ){
	(void)entry;

}

void	Logs::deleteLog( log_t& entry ){
	(void)entry;
}

void	Logs::clearLog( log_t& entry ){
	(void)entry;
}

str_t	Logs::getType_name( int entry_type ){
	
	str_t name;
	
	switch (entry_type){
		case LOGS_ERR	: { name = "ERROR"; return name;}
		case LOGS_CONF	: { name = "CONFIG"; return name;}
		case LOGS_SERV	: { name = "SERVER"; return name;}
		case LOGS_REQU	: { name = "REQUEST"; return name;}
		case LOGS_RESP	: { name = "RESPONSE"; return name;}
		case LOGS_DEBUG : { name = "DEBUG"; return name;}
		default 		: {name = "DEFAULT"; return name;}
	}
}

void	Logs::openFile(str_t filename){(void)filename;}
void	Logs::clostFile(){}
