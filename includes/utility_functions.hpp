/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility_functions.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:04:15 by kmooney           #+#    #+#             */
/*   Updated: 2025/04/06 21:08:36 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITY_FUNCTIONS_HPP
#define UTILITY_FUNCTIONS_HPP

#include "core_def.hpp"

#include <cctype>
#include <cstring>
#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <vector>

#include <dirent.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>

/*  SET CREATION */
const std::set<std::string>& 		getHeaderTransferEncodings();

/* PARSING OPERATIONS */
void								parseStreamToStrStrMap(std::istringstream& iss, std::map<std::string, std::string>& result, char pair_delim, char kv_delim);
void								parseStreamToStrVecStrMap(std::istringstream& iss, std::map<std::string, std::vector< std::string> >& result, char pair_delim, char kv_delim, char vec_delim);

/* STRING CASE CONVERSION */
std::string							to_lower(std::string str);
std::string							to_upper(std::string str);
void								to_lower_ref(str_t& str);
void								to_upper_ref(str_t& str);

/* STRING MODIFICATION */
void								trimLeadingChar( std::string& str, char target);
void								trimEndChar( std::string& str, char target);
void								remove_dot_segments(std::string&);

/* STRING/CHAR TYPE CONVERSION */
int									str_to_int( const std::string str);
int									substring_to_int(const std::string str, int start, int end);
long								hexToLong(const char *hex);
unsigned char						hexCharToUnsignedChar(char c);

/* VALID CHARACTER CHECKS */
bool								isValidUTF8(const std::string& str);

/* PRINTING OPERATIONS */
void								printStrMap(const std::map<std::string, std::string>& data);
void                                printStrVecStrMap(const std::map<std::string, std::vector< std::string> >& data);

/*  FILE OPERATIONS  */
int									getFileType(const std::string& str);
bool								isAccessible(const std::string& path);
std::string&    					pathAppendToRoot(const std::string& path, const std::string& root);
std::pair<std::string, std::string>	pathSplit(std::string& path);

#endif
