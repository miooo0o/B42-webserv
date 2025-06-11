#ifndef SERVERCONFIG_HPP
 #define SERVERCONFIG_HPP

#include <map>
#include "ServerConfigBase.hpp"

class Location;
class MimeMap;
class ConfigInit;

class ServerConfig : public ServerConfigBase {
public:
	explicit ServerConfig(const ConfigInit& init);
	~ServerConfig();

 	const Location*	findLocation(const std::string& uri) const;

};

#endif //SERVERCONFIG_HPP

/**
 * vector<config> <- host + port or ListenAddress struct  /server_name
 * content type
 * - json
 * - application/octet-stream
 * - x-www-form-urlendcoded
 * - formdata
 *
 */
