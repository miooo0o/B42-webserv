#include "SimpleResponse.hpp"

/**
 * Static member initialization
 */
std::map<int, std::string> SimpleResponse::_errorStatusMaps;
bool	SimpleResponse::_isErrorMapsInitialized = false;


////////////////////////////////////////////////////////////////////////////////
// SimpleResponse Constructor & Destructor
////////////////////////////////////////////////////////////////////////////////

/**
 *
 * @param errorCode range 400 ~ 599
 */
SimpleResponse::SimpleResponse(const int errorCode) {
	_initStatusMap();
	_validateErrorCode(errorCode);
	_createResponse();
}

SimpleResponse::~SimpleResponse() {
}


////////////////////////////////////////////////////////////////////////////////
// static methods
////////////////////////////////////////////////////////////////////////////////


/**
 * @brief Static. Creates a simple HTTP error response as a string.
 * @param errorCode HTTP status code (400-599)
 * @return Formatted HTTP response including status line, headers and body
 */
std::string	SimpleResponse::simpleResponse(const int errorCode) {
	SimpleResponse simple(errorCode);
	return (simple.to_string());
}

/**
 * @brief Initializes the static map of HTTP error codes to their corresponding messages
 *        This method is called once to populate the error code dictionary
 */
void	SimpleResponse::_initStatusMap() {
	if (!_isErrorMapsInitialized) {
		_errorStatusMaps[400] = "Bad Request";
		_errorStatusMaps[401] = "Unauthorized";
		_errorStatusMaps[403] = "Forbidden";
		_errorStatusMaps[404] = "Not Found";
		_errorStatusMaps[405] = "Method Not Allowed";
		_errorStatusMaps[406] = "Not Acceptable";
		_errorStatusMaps[408] = "Request Timeout";
		_errorStatusMaps[413] = "Payload Too Large";
		_errorStatusMaps[414] = "URI Too Long";
		_errorStatusMaps[415] = "Unsupported Media Type";
		_errorStatusMaps[429] = "Too Many Requests";
		_errorStatusMaps[500] = "Internal Server Error";
		_errorStatusMaps[501] = "Not Implemented";
		_errorStatusMaps[502] = "Bad Gateway";
		_errorStatusMaps[503] = "Service Unavailable";
		_errorStatusMaps[504] = "Gateway Timeout";
		_errorStatusMaps[505] = "HTTP Version Not Supported";
		_isErrorMapsInitialized = true;
	}
}


////////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief
 * @return
 */
std::string SimpleResponse::to_string() {
	std::stringstream ss;

	ss << "HTTP/1.1 " << _errorCode << " " << _message << "\r\n";
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin();
		it != _headers.end(); ++it) {
		ss << it->first << ": " << it->second << "\r\n";
		}
	ss << "\r\n";
	ss << _body;
	return (ss.str());

}

////////////////////////////////////////////////////////////////////////////////
// Private
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Creates the HTTP response by setting headers and generating the response body
 *        Sets Content-Type, Connection headers, and calculates Content-Length
 */
void	SimpleResponse::_createResponse() {
	_headers["Content-Type"] = "text/html";
	_headers["Connection"] = "close";
	_createBody();

	std::stringstream	ss;
	ss << _body.length();

	_headers["Content-Length"] = ss.str();
}

/**
 * @brief Generates a simple HTML error page as the response body
 *        Includes the error code and message in a standardized format
 */
void	SimpleResponse::_createBody() {
	std::stringstream	bodyStream;

	bodyStream << "<html><head><title>" << _errorCode << " " << _message << "</title></head>";
	bodyStream << "<body>";
	bodyStream << "<h1>" << _errorCode << " " << _message << "</h1>";
	bodyStream << "<hr>";
	bodyStream << "<p>The server encountered an error while processing.</p>";
	bodyStream << "</body></html>";
	_body = bodyStream.str();
}


/**
 * @brief Validates and normalizes the error code
 *        Ensures the code is within valid HTTP error range (400-599)
 *        Maps the code to its corresponding message or sets "Unknown Error"
 *
 * @param errorCode HTTP status code to validate
 */
void	SimpleResponse::_validateErrorCode(const int errorCode)
	: _errorCode(errorCode) {
	if (!(_errorCode >= 400 && _errorCode < 600)) {
		_errorCode = 500;
	}
	if (_errorStatusMaps.find(_errorCode) == _errorStatusMaps.end()) {
		_message = "Unknown Error";
	} else {
		_message = _errorStatusMaps[_errorCode];
	}
}

