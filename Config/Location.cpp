#include "Location.hpp"
#include "ConfigUnit.hpp"

Location::Location()
	: ConfigUnit() {}

Location::Location(const ConfigUnit *parent)
	: ConfigUnit(parent) {
	Location::applyInheritedValuesFrom(*parent);
}

Location::~Location() {}

void	Location::applyInheritedValuesFrom(const ConfigUnit& parent) {
	ConfigUnit::applyInheritedValuesFrom(parent);

	const Location* parent_unit = dynamic_cast<const Location*>(&parent);
	if (parent_unit) {
		if (!hasOverride(HAS_URI))		_uri = parent_unit->getUri();
		if (!hasOverride(HAS_REDIRECT))	_redirect = parent_unit->getRedirect();
	}
}

