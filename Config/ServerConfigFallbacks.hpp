#ifndef SERVERCONFIGFALLBACKS_HPP
#define SERVERCONFIGFALLBACKS_HPP

#include <iostream>
#include <string>
#include <vector>

class Location;
class MimeMap;

// FIXME: default size not confirmed <- ref nginx
enum FallbackValues { CLIENT_MAX_BODY_SIZE = 4092, CLIENT_MAX_HEADER_SIZE = 8192, KEEP_ALIVE_INTERVAL = 10 };

/**
 * Default configuration values used as fallback
 */
struct ServerConfigFallbacks {
	std::string					profile;				// default: default
	std::string					root;                   // Default root directory
	std::vector<std::string>	allowMethods;           // Default allowed methods (e.g., GET)
	std::vector<std::string>	indexFiles;             // Default index files (e.g., index.html)
	static MimeMap				defaultMap;             // Fallback MIME type map

	size_t						client_max_body_size;   // Max body size in bytes (default: CLIENT_MAX_BODY_SIZE)
	size_t						client_max_header_size; // Max header size in bytes (default: CLIENT_MAX_HEADER_SIZE)

	bool						autoindex;              // If true, enable directory listing (default: false)

	// ============================================================================================================
	// Server-Level Control Flags. These override Location-level settings.
	// ============================================================================================================

	bool						keepAlive;              // If true, keep-alive is supported (default: true)
	size_t						keepAliveTimeout;       // Keep-alive timeout in seconds (default: KEEP_ALIVE_INTERVAL)
	bool						chunkedSupport;         // Whether chunked transfer is supported (default: true)
	bool						cgi_support;			// if CGI is enabled (default: false)
	std::string					cgi_path;               // Default CGI execution path (empty if not set)
	std::string					default_type;			// (default: application/octet-stream)
	bool						serverTokens;           // If false, hide server info in responses (default: false)


	explicit ServerConfigFallbacks() :
		profile("default"), root("/var/www/html"), client_max_body_size(CLIENT_MAX_BODY_SIZE),
		client_max_header_size(CLIENT_MAX_HEADER_SIZE), autoindex(false), keepAlive(true),
		keepAliveTimeout(KEEP_ALIVE_INTERVAL), chunkedSupport(true), serverTokens(false),
		default_type("application/octet-stream"), cgi_support(false) {
		allowMethods.push_back("GET");
		indexFiles.push_back("index.html");
		cgi_path = "";
	}
};


/**
 * todo: documentation
 */
class FallbackInjector {
public:
	static void applyProfile(ServerConfigFallbacks& f, const std::string& profile);
	static ServerConfigFallbacks buildWithProfile(const std::string& profile);

private:
	struct ProfileHandler {
		const char* name;
		void (*func)(ServerConfigFallbacks&);
	};

	static const ProfileHandler handlers[];
	static void inject_default(ServerConfigFallbacks& f);
	static void inject_test(ServerConfigFallbacks& f);
};

#endif