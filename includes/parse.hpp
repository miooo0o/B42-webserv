/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 23:04:58 by kmooney           #+#    #+#             */
/*   Updated: 2025/04/10 08:43:17 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
# define PARSE_HPP

#include <cstring>
#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <vector>

#include <stdint.h>
#include <unistd.h>

#define KEY_DELIM ":"
#define LINE_DELIM "\r\n"
#define VAL_DELIM ","

#include <iostream>
#include <string>
#include <iterator>
#include <sstream>

enum status {
	LINE_SPLIT,
	KEY_SPLIT,
	VAL_SPLIT,
	EMPTY,
	NO_KEY,
	NO_EOL,
	NO_DELIM,
	NO_STATUS,
	NO_VAL,
	EOL,
	EOS
};

enum mapType {
	ARRAY,
	MAPSTRSTR,
	MAPSTRVEC,
	VECTOR
};

#endif