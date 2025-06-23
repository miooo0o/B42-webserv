#ifndef CONFIGUNIT_HPP
 #define CONFIGUNIT_HPP

#include <map>
#include <string>
#include <vector>

#include "ConfigCommon.hpp"
#include "MimeType.hpp"

class MimeType;

class ConfigUnit {
protected:
	const ConfigUnit*			_inheritedFrom;

	std::string					_root;						// [REQUIRED]
	std::vector<std::string>	_indexFiles;
	std::vector<std::string>	_allowMethods;
	size_t						_client_max_body_size;		// Max body size in bytes
	size_t						_client_max_header_size;	// Max header size in bytes
	bool						_autoindex;					// If true, enable directory listing
	bool						_cgi_support;				// if CGI is enabled
	std::string					_cgi_path;					// CGI execution path
	std::string					_defaultType;
	MimeType*					_mimeType;

	/**
	 * @brief ConfigOverrideFlag
	 */
	unsigned int				_overrideFlags;

public:
	ConfigUnit();
	explicit ConfigUnit(const std::string& root);
	explicit ConfigUnit(const ConfigUnit* parent);

	virtual ~ConfigUnit() {}

	// override handle methods
	bool	hasOverride(ConfigOverrideFlag flag) const;
	void	setOverride(ConfigOverrideFlag flag);
	void	resetOverride(const unsigned int value = 0) { _overrideFlags = value; }
	void	unsetOverride(ConfigOverrideFlag flag);

	// getter/setter for `_inheritedFrom`
	void				setInheritedValues(const ConfigUnit* fb);
	const ConfigUnit*	getInheritedValues() const;

	// getter/setter for `_mimeType`

	// accessor (getter/setter)
	CONFIG_OVERRIDE_ACCESSOR(root, Root, std::string,HAS_ROOT, ConfigUnit)
	CONFIG_OVERRIDE_ACCESSOR(indexFiles, IndexFiles, std::vector<std::string>, HAS_INDEX_FILES, ConfigUnit)
	CONFIG_OVERRIDE_ACCESSOR(allowMethods, AllowMethods, std::vector<std::string>, HAS_ALLOW_METHODS, ConfigUnit)
	CONFIG_OVERRIDE_ACCESSOR(client_max_body_size, ClientMaxBodySize, size_t, HAS_CLIENT_MAX_BODY_SIZE, ConfigUnit)
	CONFIG_OVERRIDE_ACCESSOR(client_max_header_size, ClientMaxHeaderSize, size_t, HAS_CLIENT_MAX_HEADER_SIZE, ConfigUnit)
	CONFIG_OVERRIDE_ACCESSOR(autoindex, Autoindex, bool, HAS_AUTOINDEX, ConfigUnit)
	CONFIG_OVERRIDE_ACCESSOR(cgi_support, CgiSupport, bool, HAS_CGI_SUPPORT, ConfigUnit)
	CONFIG_OVERRIDE_ACCESSOR(cgi_path, CgiPath, std::string, HAS_CGI_PATH, ConfigUnit)
	CONFIG_OVERRIDE_ACCESSOR(defaultType, DefaultType, std::string, HAS_DEFAULT_TYPE, ConfigUnit)


	virtual void		applyInheritedValuesFrom(const ConfigUnit& fb);

	static ConfigUnit*	baseDefaults();
};

#endif //CONFIGUNIT_HPP
