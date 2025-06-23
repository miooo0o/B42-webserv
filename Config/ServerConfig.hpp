#ifndef SERVERCONFIG_HPP
 #define SERVERCONFIG_HPP

#include <map>
#include <string>
#include <vector>

#include "ConfigCommon.hpp"
#include "ConfigParser.hpp"
#include "ConfigUnit.hpp"
#include "Location.hpp"

enum AddressFamily {
	NON_AF_SET,
	AF_IPV4,
	AF_IPV6
};

struct ListenAddress {
	AddressFamily	family;
	std::string		host;	// e.g., "0.0.0.0" or "::"
	uint16_t		port;	// e.g., 8080

	ListenAddress() : family(NON_AF_SET), host(ConfigDefaults::LISTEN_HOST), port(ConfigDefaults::LISTEN_PORT) {}
};

typedef std::map<int, std::string>		ErrorPagesMap;
typedef std::map<std::string, Location>	LocationsMap;

class ServerConfig : public ConfigUnit {
private:
	ListenAddress					_listen;				// [REQUIRED]
	std::vector<std::string>		_serverNames;
	LocationsMap					_locations;
	ErrorPagesMap					_errorPages;

	bool							_chunkedSupport;		// default: true
	bool							_serverTokens;			// show version info in header (default: false)
	bool							_keepAlive;				// default: true
	size_t							_keepAliveTimeout;		// default: 10s


public:
	ServerConfig();
	ServerConfig(const ConfigUnit *parent);
	virtual ~ServerConfig() {}

	virtual void applyInheritedValuesFrom(const ConfigUnit& parent);	// overrride

	// static, base defaults set
	static ServerConfig*		baseDefaults();

	CONFIG_OVERRIDE_ACCESSOR(listen, Listen, ListenAddress, HAS_LISTEN, ServerConfig)
	CONFIG_OVERRIDE_ACCESSOR(errorPages, ErrorPages, ErrorPagesMap, HAS_ERROR_PAGES, ServerConfig)


	// add
	void							addServerName(const std::string& name);
	void							addLocation(Location loc);

	// find
	const Location*					findLocation(const std::string& uri) const;

	// get
	const std::vector<std::string>&	getServerNames() const;
	const LocationsMap&				getLocations() const;

	// static

};

#endif // SERVERCONFIG_HP