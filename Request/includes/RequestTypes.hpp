/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestTypes.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:24:23 by kmooney           #+#    #+#             */
/*   Updated: 2025/05/14 13:23:58 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_TYPES_HPP
# define REQUEST_TYPES_HPP

#include "core_def.hpp"
#include <string>
#include <vector>
#include <map>

enum eParse { P_PARSE, P_AWAIT, P_UNPROC, P_DONE, P_ERROR };
enum eReqState { P_REQLINE, V_REQLINE, P_HEADERS, V_HEADERS, P_BODY, V_BODY, REQ_DONE };
enum states { SCHEME, USER, HOST, PORT, PATH, QUERY, FRAG, URI_END };
enum uri_types{ AUTH_FORM, ORIGIN_FORM, ABSOLUTE_FORM, ASTERISK };

typedef struct {
	std::string val;
	std::vector<std::pair<std::string, std::string> >params;
} HeaderField_t;

typedef std::map<std::string, std::vector<HeaderField_t* > > headerMap_t;
typedef std::vector< std::pair<std::string, std::string> > query_t;

typedef struct uri_s {
	enum uri_types			uri_type;
	size_t					len;
	int						port_int;
	str_t 					frag, host, pass, path, port, query, scheme, str, target, user;
	query_t					queries;

	uri_s()
		: uri_type(ORIGIN_FORM), len(0), port_int(80), frag(""), host(""), 
			pass(""), path(""), port(""), query(""), scheme(""), str(""), user(""){}
} uri_t;

typedef struct  {
	std::string	contentType;
    std::string	disposition;
    std::string	filename;
    bool		isFile;
    std::string	tempFilePath;
    std::vector<char> content;
} MultipartPart;

typedef struct {
	std::vector<MultipartPart>	parts;
	std::string					boundary;
} MultipartMessage;

#endif