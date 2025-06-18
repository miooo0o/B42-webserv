#include "Location.hpp"
#include "ServerConfigFallbacks.hpp"
Location::Location() {
	const ServerConfigFallbacks	fallbacks;
	_indexFiles = fallbacks.indexFiles;
	_client_max_header_size = fallbacks.client_max_header_size;
	_client_max_body_size = fallbacks.client_max_body_size;
	_autoindex = fallbacks.autoindex;
	_cgi_path = fallbacks.cgi_path;
	_allowMethods = fallbacks.allowMethods;
	_default_type =	fallbacks.default_type;

}

Location	Location::applyFallbackFrom(const ServerConfigFallbacks& fallbacks) {

}

Location	Location::overrideWith(const ServerConfigFallbacks& fallbacks) {

}