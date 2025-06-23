#ifndef MIMETYPE_HPP
 #define MIMETYPE_HPP

#include <map>
#include <set>
#include <exception>
typedef std::map<std::string, std::string> MimeTypeMap;

class MimeRegistry {
private:
	static MimeTypeMap	_mimeMap;

public:
	MimeRegistry();
	// explicit MimeRegistry(const std::string& path);

	static std::string	lookupType(const std::string& ext);
	static MimeTypeMap&	getMap();

private:
	// static void			loadFromFile(const std::string& path);	// from parser?
};

class ProfiledMimeTypes {
private:
	static std::map<std::string, MimeTypeMap>	_mimeRegistryMap;
	static std::set<std::string>				_availableProfiles;
public:
	ProfiledMimeTypes();
	static MimeTypeMap&	loadDefaultMimeMap();
	// static MimeTypeMap&	loadMimeMap(const std::string& profile, const std::string& ext);
};
 
#endif //MIMETYPE_HPP
