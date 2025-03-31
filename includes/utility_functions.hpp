/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility_functions.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:04:15 by kmooney           #+#    #+#             */
/*   Updated: 2025/03/30 20:14:49 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITY_FUNCTIONS_HPP
#define UTILITY_FUNCTIONS_HPP

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
const std::set<std::string>&		get_unsupported_schemes();

/* PARSING OPERATIONS */
void								parseStrStreamToMap(std::istringstream& iss, std::map<std::string, std::string>& result, char pair_delim, char kv_delim);

/* STRING CASE CONVERSION */
std::string							to_lower(std::string str);
std::string							to_upper(std::string str);
void								to_lower_ref(std::string& str);
void								to_upper_ref(std::string& str);

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

/*  FILE OPERATIONS  */
int									getFileType(const std::string& str);
bool								isAccessible(const std::string& path);
std::string&    					pathAppendToRoot(const std::string& path, const std::string& root);
std::pair<std::string, std::string>	pathSplit(std::string& path);

#endif
