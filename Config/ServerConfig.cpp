#include "ServerConfig.hpp"

ServerConfig::ServerConfig() : ConfigUnit() { applyInheritedValuesFrom(*this); }

void	ServerConfig::addServerName(const std::string& name) {
	_serverNames.push_back(name);

}
const std::vector<std::string>& ServerConfig::getServerNames() const {
	return _serverNames;
}

void	ServerConfig::addLocation(Location loc) {
	if (!loc.getUri().empty()) {
		_locations[loc.getUri()] = loc;
	}
}

const LocationsMap& ServerConfig::getLocations() const {
	return _locations;
}

const Location* ServerConfig::findLocation(const std::string& uri) const {
	std::map<std::string, Location>::const_iterator it = _locations.find(uri);
	return it != _locations.end() ? &it->second : NULL;
}

void	ServerConfig::applyInheritedValuesFrom(const ConfigUnit& parent) {
	ConfigUnit::applyInheritedValuesFrom(parent);

	const ServerConfig* parent_unit = dynamic_cast<const ServerConfig*>(&parent);
	if (parent_unit) {
		if (!hasOverride(HAS_LISTEN))			_listen = parent_unit->getListen();
		if (!hasOverride(HAS_ERROR_PAGES))	_errorPages = parent_unit->getErrorPages();
	}
}

ServerConfig*	ServerConfig::baseDefaults() {
	static ServerConfig instance(ConfigUnit::baseDefaults());
	instance.setListen(ListenAddress());

	return &instance;
}