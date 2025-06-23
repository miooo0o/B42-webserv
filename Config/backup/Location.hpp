#ifndef LOCATION_HPP
 #define LOCATION_HPP

#include <map>
#include <string>
#include <vector>

#include "../ConfigParser.hpp"


class Location {
private:
	std::vector<std::string> 		_indexFiles;			// [OPTIONAL] fallback to default (e.g. index.html)
	size_t							_client_max_header_size;// [OPTIONAL] max limit; fallback or clamp in Validator
	size_t							_client_max_body_size;	// [OPTIONAL] same as above

	bool							_autoindex;				// [OPTIONAL] default: false

	std::string						_cgi_path;				// [OPTIONAL] if CGI is enabled (default: "")
	RedirectInfo					_redirectInfo;			// [OPTIONAL] if return code present

	std::map<int, std::string>		_errorPageMap;			// [OPTIONAL] if empty, fallback or ignore if not found
	std::vector<std::string>		_allowMethods;			// [OPTIONAL] if empty, fallback to default: GET
	std::map<std::string, Location>	_locationMap;			// [OPTIONAL] if empty, fallback to server-level config
	// MimeMap 						_mimeMap;				// [OPTIONAL] if empty, fallback to defaultMimeMap
	std::string						_default_type;			// [OPTIONAL] if empty, fallback to default: application/octet-stream;

public:
	explicit Location();

	static Location applyFallbackFrom(const ServerConfigFallbacks& fallbacks);
	static Location	overrideWith(const ServerConfigFallbacks& fallbacks);
};



#endif //LOCATION_HPP
