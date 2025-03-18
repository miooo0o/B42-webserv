#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>
#include <iostream>

class ResponseState;
class ContentHandler;
class Request;
class Entrise;

#include "EntryObserver.hpp"
#include "Request.hpp"

class Response : public EntryObserver {
private:
	Request&							_request;
	Entries*							_entries; 

	std::map<std::string, std::string>  _headers;
	std::string                         _body;

public:
	Response(Request& request, Entries* entries);		/* with params */
	~Response();

	Response		to_response();
	
	/* ... */
	ResponseState*	createState();
	bool			evaluate();

	/* to_string*/
	std::string		to_string();

	/* setter */
	void			setState(int code);
	void			setBody(const std::string& content);

	/* getter */
	std::string							getBody() const;
	std::string							getStatus() const;
	std::map<std::string, std::string>	getHeaders() const;

	/* add methods */
	void		addHeader(const std::string& key, const std::string& value);
};

#endif 