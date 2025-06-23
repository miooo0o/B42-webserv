#include "ConfigUnit.hpp"

ConfigUnit::ConfigUnit()
	: _inheritedFrom(ConfigUnit::baseDefaults()) { applyInheritedValuesFrom(*_inheritedFrom); }


ConfigUnit::ConfigUnit(const std::string& root)
	: _inheritedFrom(ConfigUnit::baseDefaults()) {
	applyInheritedValuesFrom(*_inheritedFrom);
	this->setRoot(root);
}


ConfigUnit::ConfigUnit(const ConfigUnit* parent)
	: _inheritedFrom(parent ? parent : ConfigUnit::baseDefaults()),
	  _overrideFlags(0)
{
	applyInheritedValuesFrom(*ConfigUnit::baseDefaults());
	if (parent)
		applyInheritedValuesFrom(*parent);
}

bool	ConfigUnit::hasOverride(const ConfigOverrideFlag flag) const {
	return (_overrideFlags & flag) != 0;
}

void	ConfigUnit::setOverride(const ConfigOverrideFlag flag) {
	_overrideFlags |= flag;
}

void	ConfigUnit::unsetOverride(const ConfigOverrideFlag flag) {
	_overrideFlags &= ~flag;
}

void	ConfigUnit::setInheritedValues(const ConfigUnit* fb) {
	_inheritedFrom = fb;
}

const ConfigUnit*	ConfigUnit::getInheritedValues() const {
	return _inheritedFrom;
}

void	ConfigUnit::applyInheritedValuesFrom(const ConfigUnit& fb) {
	if (!hasOverride(HAS_ROOT))                     _root = fb.getRoot();
	if (!hasOverride(HAS_INDEX_FILES))              _indexFiles = fb.getIndexFiles();
	if (!hasOverride(HAS_ALLOW_METHODS))            _allowMethods = fb.getAllowMethods();
	if (!hasOverride(HAS_CLIENT_MAX_BODY_SIZE))     _client_max_body_size = fb.getClientMaxBodySize();
	if (!hasOverride(HAS_CLIENT_MAX_HEADER_SIZE))   _client_max_header_size = fb.getClientMaxHeaderSize();
	if (!hasOverride(HAS_AUTOINDEX))                _autoindex = fb.getAutoindex();
	if (!hasOverride(HAS_DEFAULT_TYPE))             _defaultType = fb.getDefaultType();
	if (!hasOverride(HAS_CGI_PATH))                 _cgi_path = fb.getCgiPath();
}

ConfigUnit*	ConfigUnit::baseDefaults() {
	static ConfigUnit instance;

	instance.resetOverride();
	instance.setRoot(ConfigDefaults::ROOT, false);
	instance.setIndexFiles(ConfigDefaults::getDefaultIndexFiles() ,false);
	instance.setAllowMethods(::ConfigDefaults::getDefaultAllowMethods() ,false);
	instance.setClientMaxBodySize(ConfigDefaults::MAX_BODY_SIZE ,false);
	instance.setClientMaxHeaderSize(ConfigDefaults::MAX_HEADER_SIZE ,false);
	instance.setAutoindex(ConfigDefaults::AUTOINDEX ,false);
	instance.setCgiSupport(ConfigDefaults::CGI_SUPPORT ,false);
	instance.setCgiPath(ConfigDefaults::CGI_PATH ,false);
	instance.setDefaultType(ConfigDefaults::DEFAULT_TYPE ,false);
	instance.setInheritedValues(&instance);
	return &instance;
}
