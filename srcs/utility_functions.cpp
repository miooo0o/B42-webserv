/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility_functions.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:04:15 by kmooney           #+#    #+#             */
/*   Updated: 2025/04/06 12:53:06 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utility_functions.hpp"

const std::set<std::string> &getHeaderTransferEncodings()
{

	static std::set<std::string> supportedEndodings;
	static int i;

	if (i == 0)
	{
		supportedEndodings.insert(std::string("chunked"));
		// supportedEndodings.insert(std::string("compress"));
		// supportedEndodings.insert(std::string("deflate"));
		// supportedEndodings.insert(std::string("gzip"));
		// supportedEndodings.insert(std::string("identity"));
		// supportedEndodings.insert(std::string("trailers"));
		// supportedEndodings.insert(std::string("x-compress"));
		// supportedEndodings.insert(std::string("x-gzip"));
		i++;
	}
	return supportedEndodings;
}

/* PRINTING OPERATIONS */

void printStrMap(const std::map<std::string, std::string> &data)
{

	std::cout << "PARSED HEADER MAP \n===================" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = data.begin(); it != data.end(); it++)
	{
		int i = 0;
		std::cout << "Key : " + it->first << std::endl;
		if (it->second == "\r")
		{
			std::cout << "[CR]" << std::endl
					  << std::endl;
		}
		else
		{
			std::cout << "Val " << i << " : " + it->second << std::endl;
			i++;
		}
		std::cout << std::endl;
	}
	std::cout << "END OF HEADER MAP \n===================" << std::endl
			  << std::endl;
}

void printStrVecStrMap(const std::map<std::string, std::vector<std::string> > &data) // NEED TO CONSIDER HOW VECTOR FIELDS ARE POPULATED
{
	std::cout << "PARSED HEADER MAP VECTOR \n==========================" << std::endl;
	for (std::map<std::string, std::vector<std::string> >::const_iterator it = data.begin(); it != data.end(); it++)
	{
		int i = 0;
		std::cout << "Key : " + it->first << std::endl;
		for (std::vector<std::string>::const_iterator itVec = it->second.begin(); itVec != it->second.end(); itVec++)
		{
			if (*itVec == "\r")
			{
				std::cout << "[CR]" << std::endl
						  << std::endl;
			}
			else
			{
				std::cout << "Val " << i << " : " + *itVec << std::endl;
			}
			i++;
		}
		std::cout << std::endl;
	}
	std::cout << "END OF HEADER MAP \n===================" << std::endl
			  << std::endl;
}

void parseStreamToStrVecStrMap(std::istringstream &iss, std::map<std::string, std::vector<std::string> > &result, char pair_delim, char kv_delim, char vec_delim)
{
	std::string line;
	std::string key;
	std::string value;

	while (std::getline(iss, line, pair_delim))
	{
		std::size_t pos = line.find(kv_delim);
		if (pos == std::string::npos)
			break;
		key = line.substr(0, pos);
		value = line.substr(pos + 1);
		std::istringstream vec_stream(value);
		while (std::getline(vec_stream, line, vec_delim))
		{
			result[key].push_back(line);
		}
	}
	result["mapLastLine"].push_back(line);
	printStrVecStrMap(result);
}

void parseStreamToStrStrMap(std::istringstream &iss, std::map<std::string, std::string> &result, char pair_delim, char kv_delim)
{
	std::string line;
	std::string key;
	std::string value;

	while (std::getline(iss, line, pair_delim))
	{
		std::size_t pos = line.find(kv_delim);
		if (pos == std::string::npos)
			break;
		key = line.substr(0, pos);
		value = line.substr(pos + 1);
		result[key] = value;
	}
	result["mapLastLine"] = line;
	printStrMap(result);
}

std::vector<std::string> split_path(std::istringstream &iss)
{
	std::string str;
	std::vector<std::string> segments;

	if (str.substr(0, 1) == "./")
		str = str.substr(2);
	else if (str[0] == '/')
		segments.push_back("/");
	while (std::getline(iss, str, '/'))
	{
		if (str.empty() || str.compare(".") == 0)
			continue;
		else if (str.compare("..") == 0 && segments.size() > 1)
			segments.pop_back();
		else
			segments.push_back(str);
	}
	return segments;
}

std::string rebuild_path(std::vector<std::string> segments)
{
	std::string path;

	if (segments[0] == "/")
		for (size_t i = 1; i < segments.size(); i++)
		{
			path + segments[i];
		}
	return path;
}

void remove_dot_segments(std::string &str)
{
	std::istringstream iss(str);
	std::vector<std::string> segments;

	segments = split_path(iss);
	str = rebuild_path(segments);
	return;
}

/* STRING CASE CONVERSION */

std::string to_lower(std::string str)
{
	for (size_t i = 0; i < str.size(); ++i)
		str[i] = std::tolower(static_cast<unsigned char>(str[i]));
	return str;
}

std::string to_upper(std::string str)
{
	for (size_t i = 0; i < str.size(); ++i)
		str[i] = std::toupper(static_cast<unsigned char>(str[i]));
	return str;
}

void to_lower_ref(std::string &str)
{
	for (size_t i = 0; i < str.size(); ++i)
		str[i] = std::tolower(static_cast<unsigned char>(str[i]));
}

void to_upper_ref(std::string &str)
{
	for (size_t i = 0; i < str.size(); i++)
		str[i] = std::toupper(static_cast<unsigned char>(str[i]));
}

/* STRING MODIFICATION */

void trimLeadingChar(std::string &str, char target)
{
	while (str[0] == target)
		str.erase(0, 1);
}

void trimEndChar(std::string &str, char target)
{
	while (str[str.size() - 1] == target)
		str.erase(str.size() - 1);
}

/* STRING/CHAR TYPE CONVERSION */

int str_to_int(const std::string str)
{
	int value;

	std::istringstream iss(str);
	iss >> value;
	return value;
}

int substring_to_int(const std::string str, int start, int end)
{
	std::string substring = str.substr(start, end);
	return (str_to_int(substring));
}

long hexToLong(const char *str)
{
	long result = 0;
	int value;
	char c;

	if (!str)
		return 0;

	while (*str)
	{
		c = *str;
		value = 0;

		if (std::isdigit(c))
			value = c - '0';
		else if (c >= 'A' && c <= 'F')
			value = c - 'A' + 10;
		else if (c >= 'a' && c <= 'f')
			value = c - 'a' + 10;
		else
			break;
		result = (result << 4) | value;
		str++;
	}

	return result;
}

unsigned char hexCharToUnsignedChar(char c)
{
	unsigned char x;

	if (c >= '0' && c <= '9')
		x = c - '0';
	else if (c >= 'A' && c <= 'F')
		x = c - 'A' + 10;
	else if (c >= 'a' && c <= 'f')
		x = c - 'a' + 10;
	else
		return -1;
	return x;
}

/* VALID CHARACTER CHECKS */

bool isValidUTF8(const std::string &str)
{
	if (str.empty())
		return true;

	unsigned char c;
	int expectedContinuationBytes = 0;
	size_t i = 0;

	while (str[i])
	{
		c = static_cast<unsigned char>(str[i]);

		if (expectedContinuationBytes == 0)
		{
			if ((c & 0x80) == 0x00)
				expectedContinuationBytes = 0;
			else if ((c & 0xE0) == 0xC0)
				expectedContinuationBytes = 1;
			else if ((c & 0xF0) == 0xE0)
				expectedContinuationBytes = 2;
			else if ((c & 0xF8) == 0xF0)
				expectedContinuationBytes = 3;
			else
				return false;
		}
		else
		{
			if ((c & 0xC0) != 0x80)
				return false;
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

/* FILE OPERATIONS */

int getFileType(const std::string &path)
{
	struct stat fileStat;

	if (stat(path.c_str(), &fileStat) == 0)
	{
		if (S_ISDIR(fileStat.st_mode))
			return 1;
		if (S_ISREG(fileStat.st_mode))
			return 2;
	}
	return 0;
}

bool isAccessible(const std::string &path)
{
	return access(path.c_str(), R_OK) == 0;
}
