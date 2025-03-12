/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:04:15 by kmooney           #+#    #+#             */
/*   Updated: 2025/02/11 16:04:33 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"

Response::Response( void ) {
	
	classText( std::cout << GRE "Constructor called (Response)" << std::endl);
}

Response::~Response( void ) {
	
	classText( std::cout << BLU "Destructor called (Response)" << std::endl);
}

Response::Response( const Response& other ) {

	( void )other;
	classText( std::cout << GRE "Copy Constructor called (Response)" << std::endl);
}

Response& Response::operator=( const Response& other ) {
	
	classText( std::cout << GRE "Copy Assignment operator called (Response)" << std::endl);
	if ( this != &other )
	{
		return ( *this );
	}
	return ( *this );
}
