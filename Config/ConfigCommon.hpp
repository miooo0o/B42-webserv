#ifndef CONFIGCOMMON_HPP
 #define CONFIGCOMMON_HPP

#include <vector>

// ============================================================================================================
// Override
// ============================================================================================================

/**
 * @brief Bitmask flags indicating which configuration fields have been explicitly overridden.
 *
 * These flags are used to determine whether a value should be taken from the current ConfigUnit
 * or inherited from a fallback. Each field that can be overridden should have a corresponding flag.
 */
enum ConfigOverrideFlag {
	HAS_ROOT                  = 1 << 0,
	HAS_INDEX_FILES           = 1 << 1,
	HAS_ALLOW_METHODS         = 1 << 2,
	HAS_CLIENT_MAX_BODY_SIZE  = 1 << 3,
	HAS_CLIENT_MAX_HEADER_SIZE= 1 << 4,
	HAS_AUTOINDEX             = 1 << 5,
	HAS_CGI_SUPPORT           = 1 << 6,
	HAS_CGI_PATH              = 1 << 7,
	HAS_DEFAULT_TYPE          = 1 << 8,
	HAS_MIME_TYPE              = 1 << 9,
	HAS_URI                   = 1 << 10,
	HAS_REDIRECT			  = 1 << 11,
	HAS_LISTEN			      = 1 << 12,
	HAS_ERROR_PAGES			  = 1 << 13
};

/**
* @note NEVER assign to _FieldName directly outside of initialization or default setup.
 *       Always use the corresponding setter (e.g., setRoot()) to ensure the appropriate
 *       override flag is correctly set. Failing to do so will break fallback behavior.
 */
#define CONFIG_OVERRIDE_ACCESSOR(FieldName, GetterName, Type, Flag, ThisClass)   	\
	Type get##GetterName() const {													\
		const ThisClass* current = this;											\
		while (current) {															\
			if (current->hasOverride(Flag))											\
				return current->_##FieldName;										\
			if (current->_inheritedFrom == current) break;							\
			current = dynamic_cast<const ThisClass*>(current->_inheritedFrom);		\
		}																			\
		return Type();																\
	}																				\
	void set##GetterName(const Type& value, bool override = false) {              	\
		if (override) setOverride(Flag);                                          	\
		_##FieldName = value;                                                     	\
	}


// ============================================================================================================
// Default values: Struct
// ============================================================================================================

struct ConfigDefaults {
	// Primitive default values
	static const char*  ROOT;
	static const char*  DEFAULT_TYPE;
	static const char*  CGI_PATH;
	static const size_t MAX_BODY_SIZE;
	static const size_t MAX_HEADER_SIZE;
	static const bool   AUTOINDEX;
	static const bool   CGI_SUPPORT;

	// Default allow methods
	static const char*  ALLOW_METHODS[];
	static std::vector<std::string> getDefaultAllowMethods();

	// Default index files
	static const char*  INDEX_FILES[];
	static std::vector<std::string> getDefaultIndexFiles();

	// Redirection
	static const int	REDIRECT_CODE;

	// Listen address
	static const char*		LISTEN_HOST;
	static const uint16_t	LISTEN_PORT;

};


// ============================================================================================================
// Default values: Primitive values
// ============================================================================================================

const char*		ConfigDefaults::ROOT = "";
const char*		ConfigDefaults::DEFAULT_TYPE = "application/octet-stream";
const char*		ConfigDefaults::CGI_PATH = "";

const size_t	ConfigDefaults::MAX_BODY_SIZE = 1000000;
const size_t	ConfigDefaults::MAX_HEADER_SIZE = 8192;

const bool		ConfigDefaults::AUTOINDEX = false;
const bool		ConfigDefaults::CGI_SUPPORT = false;

const int		ConfigDefaults::REDIRECT_CODE = 302;

const char*		ConfigDefaults::LISTEN_HOST = "127.0.0.1";
const uint16_t	ConfigDefaults::LISTEN_PORT = 8080;

// ============================================================================================================
// Default values: vector init methods
// ============================================================================================================

// Primitive value: allow methods
const char*  ConfigDefaults::ALLOW_METHODS[] = { "GET", "POST", "DELETE" };

std::vector<std::string> ConfigDefaults::getDefaultAllowMethods() {
	std::vector<std::string> methods;
	const size_t count = sizeof(ALLOW_METHODS) / sizeof(ALLOW_METHODS[0]);
	for (size_t i = 0; i < count; ++i)
		methods.push_back(ALLOW_METHODS[i]);
	return methods;
}

// Primitive value: index files
const char*  ConfigDefaults::INDEX_FILES[] = { "index.html" };

std::vector<std::string> ConfigDefaults::getDefaultIndexFiles() {

	std::vector<std::string> files;
	const size_t count = sizeof(INDEX_FILES) / sizeof(INDEX_FILES[0]);
	for (size_t i = 0; i < count; ++i)
		files.push_back(INDEX_FILES[i]);
	return files;
}


#endif //CONFIGCOMMON_HPP
