/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:57:40 by kmooney           #+#    #+#             */
/*   Updated: 2025/02/11 16:03:18 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "./TestClasses/testUtils.hpp"
#include <iostream>
#include <stdint.h>

class	Response
{	
	private:
								/* CONSTRUCTORS */
								Response( void );
								Response( const Response& other );

								/* DESTRUCTORS */
								~Response( void );

								/* COPY ASSIGNMENT */
			Response&			operator=(const Response& other);   
	
	public:
		
};

#endif