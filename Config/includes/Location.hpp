#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "ConfigCommon.hpp"
#include "ConfigUnit.hpp"

struct RedirectInfo {
	bool        enabled;
	int         code;
	std::string destination;

	RedirectInfo() : enabled(false), code(ConfigDefaults::REDIRECT_CODE), destination("") {}
};

class Location : public ConfigUnit {
private:
	std::string		_uri;
	RedirectInfo	_redirect;

public:
	Location();
	Location(const ConfigUnit *parent);
	virtual ~Location();

	virtual void applyInheritedValuesFrom(const ConfigUnit& parent);	// overrride

	CONFIG_OVERRIDE_ACCESSOR(uri, Uri, std::string, HAS_URI, Location)
	CONFIG_OVERRIDE_ACCESSOR(redirect, Redirect, RedirectInfo, HAS_REDIRECT, Location)
};

#endif // LOCATION_HPP