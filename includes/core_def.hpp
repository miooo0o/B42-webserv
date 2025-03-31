/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_def.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 18:51:39 by kmooney           #+#    #+#             */
/*   Updated: 2025/03/31 08:46:43 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_DEF_HPP
# define CORE_DEF_HPP

#include <iostream>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

typedef	int						fd_t;
typedef	std::string				str_t;

typedef struct	file_s			file_t;
typedef struct	targetInfo_s	targetInfo_t;

struct file_s {
	fd_t    	fd;
	__mode_t	perm;
	str_t   	filename;
	void		*data;
	
	file_s(str_t name) : filename(name){}
	file_s(){}
};

#endif