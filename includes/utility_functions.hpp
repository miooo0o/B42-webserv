/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility_functions.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:04:15 by kmooney           #+#    #+#             */
/*   Updated: 2025/03/12 16:26:00 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITY_FUNCTIONS_HPP
#define UTILITY_FUNCTIONS_HPP

#include <cctype>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <poll.h>
#include <set>
#include <sstream>
#include <stdint.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

const			std::set<std::string>& get_valid_methods();
const			std::set<std::string>& get_unsupported_methods();

long			hexToLong(const char *hex);
unsigned char   hexCharToUnsignedChar(char c);
bool			isValidUTF8(const std::string& str);
int				str_to_int( const std::string str);
int				substring_to_int(const std::string str, int start, int end);

std::string		to_lower(std::string str);
std::string		to_upper(std::string str);
void			to_lower_ref(std::string& str);
void			to_upper_ref(std::string& str);

#endif
