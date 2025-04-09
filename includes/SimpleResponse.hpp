#ifndef SIMPLERESPONSE_HPP
 #define SIMPLERESPONSE_HPP

// TODO(@mina): !!! merge to Response (-ing)

#include "ResponseState.hpp"
class Response;

class SimpleResponse {
private:
	int									_errorCode;
	std::string							_message;
	std::map<std::string, std::string>  _headers;
	std::string                         _body;
	static std::map<int, std::string>	_errorStatusMaps;
	static bool							_isErrorMapsInitialized;

public:
	SimpleResponse(int errorCode);
	~SimpleResponse();

	static std::string	simpleResponse(int errorCode);

	std::string			to_string();

private:
	static void	_initStatusMap();
	void		_validateErrorCode(int errorCode);
	void		_createResponse();
	void		_createBody();
};

#endif
