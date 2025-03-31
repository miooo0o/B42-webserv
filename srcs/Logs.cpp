/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logs.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 17:08:11 by kmooney           #+#    #+#             */
/*   Updated: 2025/03/31 08:50:13 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Logs.hpp"

Logs::Logs( void ){
	
}

Logs::Logs( size_t level, str_t entry, size_t action ) {

}

Logs::Logs( size_t level, file_t* file,  str_t entry, size_t action ) : _level(level), _file(file){

}

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

void	Logs::addEventToLog( size_t level, size_t action ){
	
}
