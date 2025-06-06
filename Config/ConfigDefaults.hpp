#ifndef CONFIGDEDAULTS_HPP
#define CONFIGDEDAULTS_HPP

#include <map>
#include <string>

class Location;
class MimeMap;
/**
 * Default configuration values used as fallback
 */
struct ConfigDefaults {
	std::string					root;                   // Default root directory
	std::vector<std::string>	allowMethods;           // Default allowed methods (e.g., GET)
	std::vector<std::string>	indexFiles;             // Default index files (e.g., index.html)
	static MimeMap				defaultMap;             // Fallback MIME type map

	// FIXME: default size not confirmed <- ref nginx
	size_t						client_max_body_size;   // Max body size in bytes (default: 4096 = 4KB)
	size_t						client_max_header_size; // Max header size in bytes (default: 8192 = 8KB)

	bool						autoindex;              // If true, enable directory listing (default: false)

	// ============================================================================================================
	// Server-Level Control Flags. These override Location-level settings.
	// ============================================================================================================

	bool						keepAlive;              // If true, keep-alive is supported (default: true)
	size_t						keepAliveTimeout;       // Keep-alive timeout in seconds (default: 10)
	bool						chunkedSupport;         // Whether chunked transfer is supported (default: true)
	bool						serverTokens;           // If false, hide server info in responses (default: false)
	std::string					default_type;			// application/octet-stream
	bool						cgi_support;
	std::string					cgi_path;               // Default CGI execution path (empty if not set)

	ConfigDefaults() : root("/var/www/html"),
						client_max_header_size(8192),
						client_max_body_size(4096),
						autoindex(false),
						keepAlive(true),
						keepAliveTimeout(10),
						chunkedSupport(true),
						serverTokens(false),
						default_type("application/octet-stream"),
						cgi_support(false),
						cgi_path("")

	{
		allowMethods.push_back("GET");
		indexFiles.push_back("index.html");
	}
};

/**
*
*/
MimeMap ConfigDefaults::defaultMap("default");

#endif
