
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:57:35 by kmooney           #+#    #+#             */
/*   Updated: 2025/04/10 11:25:50 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "core_def.hpp"
#include "utility_functions.hpp"
#include "RequestTypes.hpp"

class RequestParser;
class RequestValidator;

class Request {
public:
    					Request(void);
    					~Request(void);

    void				parseRequest(const char* rawData, size_t length);

    eParse				getParseStatus();
    eReqState			getReqStatus();

    void 				setRstate(eReqState r_state);
    void 				setPstate(eParse p_state);

    bool 				setState(eParse p_state, std::string err_mess, unsigned int err_num);
    bool 				setState(eParse p_state, eReqState r_state, std::string err_mess, unsigned int err_num);
	
	void				setBodyType( bool hasBody, bool isChunked );
	void				setBodyEncoding( const std::string& encoding) ;
	void				setBoundaryString( const std::string& boundary );
	void				setDefaultMultiPartType( const std::string& type );
	
    RequestParser*		_parser;
    RequestValidator*	_validator;
	//Config*			_config;

    std::string 		_method, _uri_str, _version;
	uri_t				_uri;
    headerMap_t 		_headers;
    std::string 		_body;
    std::string 		_boundary;
    std::string 		_defaultMultiType;

    unsigned int		_errNum;
    std::string			_errMess;

    eParse				_p_state;
    eReqState			_r_state;
    
	uint64_t			_message_length, _content_length;

    bool 				_hasBody, _isChunked, _isMultiPart, _bodyUTF8;
};

std::ostream& l14(std::ostream& os);
std::ostream& r3(std::ostream& os);
std::ostream& operator<<(std::ostream& os, Request& request);

#endif