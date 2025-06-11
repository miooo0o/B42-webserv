#ifndef SERVERCONFIGBASE_HPP
 #define SERVERCONFIGBASE_HPP

#include <map>
#include <string>

class Location;
class MimeMap;
class ConfigInit;
class ServerConfigFallbacks;

// TODO: need?
enum CharSet {
	UTF_8
};

// todo: think about how to validate
enum AddressFamily {
	AF_IPV4,
	AF_IPV6
};

struct ListenAddress {
	AddressFamily	family;
	std::string		host;	// e.g., "0.0.0.0" or "::"
	uint16_t		port;	// e.g., 8080
};
std::ostream& operator<<(std::ostream& os, const ListenAddress& addr);

struct RedirectInfo {
	bool        enabled;
	int         code;
	std::string target;
};
std::ostream& operator<<(std::ostream& os, const RedirectInfo& rdr);

/**
 * @brief ServerConfig holds all server-level configuration values parsed from the config file.
 *
 * Field annotations follow two levels of criticality:
 *
 * [REQUIRED]
 *   - These fields must be present and valid during the parsing stage.
 *   - If any REQUIRED field is missing or malformed, the **parsing process fails**, and the program exits with EXIT_FAILURE.
 *   - Example: `listenAddress`, `root`
 *
 * [OPTIONAL]
 *   - These fields are not required for successful parsing.
 *   - If missing, they will either:
 *     - fallback to a default value, or
 *     - be validated later during the validation phase.
 *   - During the validation phase:
 *     - If an OPTIONAL field violates critical constraints (e.g. invalid size limits),
 *       the **entire Server block will be skipped**, but the program continues running.
 *
 * This distinction allows flexible configuration while ensuring that invalid server blocks
 * do not compromise the entire web server runtime.
 */
class ServerConfigBase {
private:
	std::string						_profile;
	ListenAddress					_listenAddress;			// [REQUIRED] must be defined (e.g., 0.0.0.0:8080, [::]:80)
	std::string						_root;					// [REQUIRED] fallback via default config
	std::string						_serverName;			// [OPTIONAL] duplicates allowed (Validator may warn)

	// [Location Level] Overridden by Location block if present
	std::vector<std::string> 		_indexFiles;			// [OPTIONAL] fallback to default (e.g. index.html)
	size_t							_client_max_header_size;// [OPTIONAL] max limit; fallback or clamp in Validator
	size_t							_client_max_body_size;	// [OPTIONAL] same as above

	bool							_autoindex;				// [OPTIONAL] default: false

	std::string						_cgi_path;				// [OPTIONAL] if CGI is enabled (default: "")
	RedirectInfo					_redirectInfo;			// [OPTIONAL] if return code present

	std::map<int, std::string>		_errorPageMap;			// [OPTIONAL] if empty, fallback or ignore if not found
	std::vector<std::string>		_allowMethods;			// [OPTIONAL] if empty, fallback to default: GET
	std::map<std::string, Location>	_locationMap;			// [OPTIONAL] if empty, fallback to server-level config
	MimeMap 						_mimeMap;				// [OPTIONAL] if empty, fallback to defaultMimeMap
	std::string						_default_type;			// [OPTIONAL] if empty, fallback to default: application/octet-stream;

	// [Server Level] Overrides Location-level settings.
	bool							_keepAlive;				// [OPTIONAL] default: true
	size_t							_keepAliveTimeout;		// [OPTIONAL] default: 10s
	bool							_cgi_support;			// [OPTIONAL] if CGI is enabled (default: false)
	bool							_chunkedSupport;		// [OPTIONAL] default: true
	bool							_serverTokens;			// [OPTIONAL] show version info in header (default: false)

public:
	explicit ServerConfigBase(const ServerConfigFallbacks& fallbacks);
	~ServerConfigBase();

	// Getter
	const ListenAddress&				getListenAddress() const;
	const std::string&					getRoot() const;
	const std::string&					getServerName() const;

	const std::vector<std::string>&		getIndexFiles() const;
	size_t								getClientMaxHeaderSize() const;
	size_t								getClientMaxBodySize() const;

	bool								isAutoindex() const;
	const std::string&					getCgiPath() const;
	const RedirectInfo&					getRedirectInfo() const;

	const std::map<int, std::string>&	getErrorPageMap() const;
	const std::vector<std::string>&		getAllowMethods() const;
	const std::map<std::string, Location>& getLocationMap() const;
	const MimeMap&						getMimeMap() const;
	const std::string&					getDefaultType() const;

	bool								isKeepAlive() const;
	size_t								getKeepAliveTimeout() const;
	bool								isCgiSupport() const;
	bool								isChunkedSupport() const;
	bool								isServerTokens() const;

	// Setter prototypes
	void	setListenAddress(const ListenAddress& addr);
	void	setRoot(const std::string& root);
	void	setServerName(const std::string& name);

	void	setIndexFiles(const std::vector<std::string>& files);
	void	setClientMaxHeaderSize(size_t size);
	void	setClientMaxBodySize(size_t size);

	void	setAutoindex(bool enabled);
	void	setCgiPath(const std::string& path);
	void	setRedirectInfo(const RedirectInfo& info);

	void	setErrorPageMap(const std::map<int, std::string>& map);
	void	setAllowMethods(const std::vector<std::string>& methods);
	void	setLocationMap(const std::map<std::string, Location>& locations);
	void	setMimeMap(const MimeMap& map);
	void	setDefaultType(const std::string& type);

	void	setKeepAlive(bool enabled);
	void	setKeepAliveTimeout(size_t timeout);
	void	setCgiSupport(bool enabled);
	void	setChunkedSupport(bool enabled);
	void	setServerTokens(bool enabled);
};

#endif //SERVERCONFIGBASE_HPP
