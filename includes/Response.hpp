#pragma once

#include "ResponseState.hpp"
#include <string>
#include <map>

class ResponseState;

class Response {
private:
	ResponseState*                      _state;
	std::map<std::string, std::string>  _headers;
	std::string                         _body;

public:
	Response();
	~Response();

	/* setter */
	void		setState(ResponseState* newState);
	void		setBody(const std::string& content);

	/* getter */
	std::string	getBody() const;
	std::string getStatus() const;
	std::map<std::string, std::string>	getHeaders() const;

	/* add methods */
	void		addHeader(const std::string& key, const std::string& value);
};
