/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility_functions.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:04:15 by kmooney           #+#    #+#             */
/*   Updated: 2025/03/12 16:26:03 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utility_functions.hpp"

const std::set<std::string>& get_valid_methods() {
	
    static std::set<std::string> valid_methods;

	valid_methods.insert(std::string("GET"));
	valid_methods.insert(std::string("POST"));
	valid_methods.insert(std::string("DELETE"));
	
    return valid_methods;
}

const std::set<std::string>& get_unsupported_methods() {
	
	static std::set<std::string> unsupported_methods;
	
	unsupported_methods.insert(std::string("HEAD"));
	unsupported_methods.insert(std::string("PUT"));
	unsupported_methods.insert(std::string("CONNECT"));
	unsupported_methods.insert(std::string("OPTIONS"));
	unsupported_methods.insert(std::string("TRACE"));
	unsupported_methods.insert(std::string("PATCH"));
	unsupported_methods.insert(std::string("LINK"));
	unsupported_methods.insert(std::string("UNLINK"));
	unsupported_methods.insert(std::string("M-SEARCH"));
	
	return unsupported_methods;
}

std::string	to_lower(std::string str) {

    for (size_t i = 0; i < str.size(); ++i) {
        str[i] = std::tolower(static_cast<unsigned char>(str[i]));
    }
	return str;
}

std::string	to_upper(std::string str) {

    for (size_t i = 0; i < str.size(); ++i) {
        str[i] = std::toupper(static_cast<unsigned char>(str[i]));
    }
	return str;
}

void	to_lower_ref(std::string& str) {

    for (size_t i = 0; i < str.size(); ++i) {
        str[i] = std::tolower(static_cast<unsigned char>(str[i]));
    }
}

void	to_upper_ref(std::string& str) {

    for (size_t i = 0; i < str.size(); i++) {
        str[i] = std::toupper(static_cast<unsigned char>(str[i]));
    }
}

int	str_to_int(const std::string str){
	int	value;

	std::istringstream	iss(str);
	iss >> value;
	return value;
}

int	substring_to_int(const std::string str, int start, int end){

	std::string	substring = str.substr(start, end);
	return (str_to_int( substring ));
}

long	hexToLong(const char *str) {
	long	result = 0;
	int		value;
	char	c;

	if (!str)
		return 0;

    while (*str) {
		c = *str;
		value = 0;

        if (std::isdigit(c)) {
            value = c - '0';
        } 
        else if (c >= 'A' && c <= 'F') {
			value = c - 'A' + 10;
        }
        else if (c >= 'a' && c <= 'f') {
			value = c - 'a' + 10;
        }
		else
			break;
        result = (result << 4) | value;
        ++str;
    }
    
    return result;
}

unsigned char   hexCharToUnsignedChar(char c){
	unsigned char x;

	if (c >= '0' && c <= '9')
		x = c - '0';
	else if (c >= 'A' && c <= 'F') {
		x = c - 'A' + 10;
	}
	else if (c >= 'a' && c <= 'f') {
		x = c - 'a' + 10;
	}
	else
		return -1;
	return x;
}


bool	isValidUTF8(const std::string& str) {
	if (str.empty()) return false;

	unsigned char	c;
	int				expectedContinuationBytes = 0;
	size_t			i = 0;

    while (str[i]) {
        c = static_cast<unsigned char>(str[i]);

		if (expectedContinuationBytes == 0) {
			if ((c & 0x80) == 0x00) {
				expectedContinuationBytes = 0;
			}
			else if ((c & 0xE0) == 0xC0) {
				expectedContinuationBytes = 1;
			}
			else if ((c & 0xF0) == 0xE0) {
				expectedContinuationBytes = 2;
			}
			else if ((c & 0xF8) == 0xF0) {
				expectedContinuationBytes = 3;
			}
			else
				return false;
		}
		else {
		    if ((c & 0xC0) != 0x80) {
		        return false;
		    }
		    expectedContinuationBytes--;
		}
		i++;
	}
	return expectedContinuationBytes == 0;
}

/*
0000	0
0001	1
0010	2
0011	3
0100	4
0101	5
0110	6
0111	7
1000	8
1001	9
1010	10	A
1011	11	B
1100	12	C
1101	13	D
1110	14	E
1111	15	F

(c & 0x80) == 0x00			 c 		 0xxx xxxx	c must start 0
			 0xxx 0000
		+	 1000 0000

(c & 0xE0) == 0xC0			 c		 110x xxxx	c must start 110
		+	 1110 0000
			 1100 0000

(c & 0xF0) == 0xE0			c		 1110 xxxx	c must start 1110
		+	 1111 0000	
			 1110 0000

(c & 0xF8) == 0xF0			c		 1111 0xxx	c must start 1111 0
		+	 1111 1000
			 1111 0000

(c & 0xC0) != 0x80			c		 10xx xxxx	if the condition is true, the byte doesn't begin 11, and isn't a valid continuation byte
		+	 1100 0000
	!=		 1000 0000
*/