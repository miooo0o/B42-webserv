#include "MimeType.hpp"

ProfiledMimeTypes::ProfiledMimeTypes() {
	if (_availableProfiles.empty()) {
		_availableProfiles.insert("default");
		_availableProfiles.insert("cgi");
	}
}

MimeTypeMap&	ProfiledMimeTypes::loadDefaultMimeMap() {
    return (ConfigDefaults::MIMETYPE_MAP);
}

// ============================================================================================================
// MimeRegistry: instance unit for ProfiledMimeTypes
// ============================================================================================================

MimeRegistry::MimeRegistry() {
    _mimeMap = ConfigDefaults::MIMETYPE_MAP;
}

// MimeRegistry::MimeRegistry(const std::string& path) {
//     loadFromFile(path);
// }


std::string MimeRegistry::lookupType(const std::string& ext) {

}

MimeTypeMap& MimeRegistry::getMap() {
    return (_mimeMap);
}

// ============================================================================================================
// MimeRegistry: private: init & load
// ============================================================================================================

// MimeRegistry::loadFromFile(const std::string& path) {

// }