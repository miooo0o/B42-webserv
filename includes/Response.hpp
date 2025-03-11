#pragma once

#include <string>
#include <map>
#include <iostream>

class ResponseState;
class ContentHandler;
class Request;
#include "ContentHandler.hpp"

class Response {
private:
	ResponseState*                      _state;
	std::map<std::string, std::string>  _headers;
	std::string                         _body;

public:
	Response(Request& request);		/* with params */
	Response(int code);				/* static */
	~Response();

	/*
	Response	response(request);
	return (response.to_response());
	*/
	Response		to_response();

	/* to_string*/
	std::string		to_string();

	/* setter */
	void			setState(int code);
	ResponseState*	createState(int code);
	void			setBody(const std::string& content);

	/* getter */
	std::string							getBody() const;
	std::string							getStatus() const;
	std::map<std::string, std::string>	getHeaders() const;

	/* add methods */
	void		addHeader(const std::string& key, const std::string& value);
};
