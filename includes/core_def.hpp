/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_def.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 18:51:39 by kmooney           #+#    #+#             */
/*   Updated: 2025/03/31 12:43:59 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_DEF_HPP
# define CORE_DEF_HPP

#include <iostream>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

typedef	int				fd_t;
typedef	std::string		str_t;
//typedef struct log_s			log_t;
//typedef struct file_s			file_t;

typedef struct targetInfo_s		targetInfo_t;

typedef struct file_s {
	fd_t    	fd;
	__mode_t	perm;
	str_t   	filename;
	void		*data;
	
	file_s(){}
	file_s(str_t name) : filename(name){}
} file_t;


/* Data Type : 0 for string, 1 for stream */
/* entry type : Type of log (DEBUG, ERROR etc) */

typedef struct log_s {
	void*			data;
	int				data_type;
	int				entry_type;
	
	log_s(void *data, int data_type, int log_type ) : data(data), data_type (data_type), entry_type(log_type){}
} log_t;

#endif