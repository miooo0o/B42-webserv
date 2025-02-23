#pragma once


#include <map>


/**
 * @class Request
 */
class Request
{
private:
	/* request line */
	std::string							_method;
	std::string							_uri;
	std::string							_httpVersion;

    /**
     * @brief HTTP Headers
     * @example key:value
     *          - Content-Type: text/html
     *          - Content-Length: 42
	 */
	std::map<std::string, std::string>	_headers;

	/**
	 * body
	 */
};