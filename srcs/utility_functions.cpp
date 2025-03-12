/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility_functions.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:04:15 by kmooney           #+#    #+#             */
/*   Updated: 2025/03/13 00:00:06 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utility_functions.hpp"

const std::set<std::string>& get_valid_methods() {
	
	static std::set<std::string> valid_methods;
	static int i = 0;

	if (i == 0)
	{
		valid_methods.insert(std::string("GET"));
		valid_methods.insert(std::string("POST"));
		valid_methods.insert(std::string("DELETE"));
		i++;
	}
	
    return valid_methods;
}

const std::set<std::string>& get_unsupported_methods() {
	
	static std::set<std::string> unsupported_methods;
	static int i = 0;

	if (i == 0)
	{
		unsupported_methods.insert(std::string("HEAD"));
		unsupported_methods.insert(std::string("PUT"));
		unsupported_methods.insert(std::string("CONNECT"));
		unsupported_methods.insert(std::string("OPTIONS"));
		unsupported_methods.insert(std::string("TRACE"));
		unsupported_methods.insert(std::string("PATCH"));
		unsupported_methods.insert(std::string("LINK"));
		unsupported_methods.insert(std::string("UNLINK"));
		unsupported_methods.insert(std::string("M-SEARCH"));
		i++;
	}
	
	return unsupported_methods;
}

const std::set<std::string>& get_unsupported_schemes() {
	
	static std::set<std::string> unsupported_schemes;
	static int i;

	if (i == 0)
	{
		unsupported_schemes.insert(std::string("aaa"));
		unsupported_schemes.insert(std::string("aaas"));
		unsupported_schemes.insert(std::string("about"));
		unsupported_schemes.insert(std::string("acap"));
		unsupported_schemes.insert(std::string("acct"));
		unsupported_schemes.insert(std::string("cap"));
		unsupported_schemes.insert(std::string("cid"));
		unsupported_schemes.insert(std::string("coap"));
		unsupported_schemes.insert(std::string("coap"));
		unsupported_schemes.insert(std::string("coap"));
		unsupported_schemes.insert(std::string("coaps"));
		unsupported_schemes.insert(std::string("coaps"));
		unsupported_schemes.insert(std::string("coaps"));
		unsupported_schemes.insert(std::string("crid"));
		unsupported_schemes.insert(std::string("data"));
		unsupported_schemes.insert(std::string("dav"));
		unsupported_schemes.insert(std::string("dict"));
		unsupported_schemes.insert(std::string("dns"));
		unsupported_schemes.insert(std::string("doi"));
		unsupported_schemes.insert(std::string("dtn"));
		unsupported_schemes.insert(std::string("example"));
		unsupported_schemes.insert(std::string("file"));
		unsupported_schemes.insert(std::string("ftp"));
		unsupported_schemes.insert(std::string("geo"));
		unsupported_schemes.insert(std::string("go"));
		unsupported_schemes.insert(std::string("gopher"));
		unsupported_schemes.insert(std::string("h323"));
		unsupported_schemes.insert(std::string("iax"));
		unsupported_schemes.insert(std::string("icap"));
		unsupported_schemes.insert(std::string("im"));
		unsupported_schemes.insert(std::string("imap"));
		unsupported_schemes.insert(std::string("info"));
		unsupported_schemes.insert(std::string("ipn"));
		unsupported_schemes.insert(std::string("ipp"));
		unsupported_schemes.insert(std::string("ipps"));
		unsupported_schemes.insert(std::string("iris"));
		unsupported_schemes.insert(std::string("iris"));
		unsupported_schemes.insert(std::string("iris"));
		unsupported_schemes.insert(std::string("iris"));
		unsupported_schemes.insert(std::string("iris"));
		unsupported_schemes.insert(std::string("jabber"));
		unsupported_schemes.insert(std::string("ldap"));
		unsupported_schemes.insert(std::string("leaptofrogans"));
		unsupported_schemes.insert(std::string("mailto"));
		unsupported_schemes.insert(std::string("mid"));
		unsupported_schemes.insert(std::string("msrp"));
		unsupported_schemes.insert(std::string("msrps"));
		unsupported_schemes.insert(std::string("mt"));
		unsupported_schemes.insert(std::string("mtqp"));
		unsupported_schemes.insert(std::string("mupdate"));
		unsupported_schemes.insert(std::string("news"));
		unsupported_schemes.insert(std::string("nfs"));
		unsupported_schemes.insert(std::string("ni"));
		unsupported_schemes.insert(std::string("nih"));
		unsupported_schemes.insert(std::string("nntp"));
		unsupported_schemes.insert(std::string("opaquelocktoken"));
		unsupported_schemes.insert(std::string("pkcs11"));
		unsupported_schemes.insert(std::string("pop"));
		unsupported_schemes.insert(std::string("pres"));
		unsupported_schemes.insert(std::string("reload"));
		unsupported_schemes.insert(std::string("rtsp"));
		unsupported_schemes.insert(std::string("rtsps"));
		unsupported_schemes.insert(std::string("rtspu"));
		unsupported_schemes.insert(std::string("service"));
		unsupported_schemes.insert(std::string("session"));
		unsupported_schemes.insert(std::string("shttp"));
		unsupported_schemes.insert(std::string("sieve"));
		unsupported_schemes.insert(std::string("sip"));
		unsupported_schemes.insert(std::string("sips"));
		unsupported_schemes.insert(std::string("sms"));
		unsupported_schemes.insert(std::string("snmp"));
		unsupported_schemes.insert(std::string("soap"));
		unsupported_schemes.insert(std::string("soap"));
		unsupported_schemes.insert(std::string("stun"));
		unsupported_schemes.insert(std::string("stuns"));
		unsupported_schemes.insert(std::string("tag"));
		unsupported_schemes.insert(std::string("tel"));
		unsupported_schemes.insert(std::string("telnet"));
		unsupported_schemes.insert(std::string("tftp"));
		unsupported_schemes.insert(std::string("thismessage"));
		unsupported_schemes.insert(std::string("tip"));
		unsupported_schemes.insert(std::string("tn3270"));
		unsupported_schemes.insert(std::string("turn"));
		unsupported_schemes.insert(std::string("turns"));
		unsupported_schemes.insert(std::string("tv"));
		unsupported_schemes.insert(std::string("urn"));
		unsupported_schemes.insert(std::string("vemmi"));
		unsupported_schemes.insert(std::string("vnc"));
		unsupported_schemes.insert(std::string("ws"));
		unsupported_schemes.insert(std::string("wss"));
		unsupported_schemes.insert(std::string("xcon"));
		unsupported_schemes.insert(std::string("xcon"));
		unsupported_schemes.insert(std::string("xmlrpc"));
		unsupported_schemes.insert(std::string("xmlrpc"));
		unsupported_schemes.insert(std::string("xmpp"));
		unsupported_schemes.insert(std::string("z39"));
		unsupported_schemes.insert(std::string("z39"));
		i++;
	}
		
	return unsupported_schemes;
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