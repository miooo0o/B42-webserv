#ifndef MIMETYPE_HPP
 #define MIMETYPE_HPP

#include <map>
#include <set>

typedef std::map<std::string, std::string> MimeMap;
class MimeType {
private:
	static std::map<std::string, std::string>	_mimeMap;

public:
	static void loadFromFile(const std::string& path);
	static std::string get(const std::string& ext);
};

class MimeTypeMap {
private:
	static std::map<std::string, MimeMap>			_mimeMap;
	static std::set<std::string>					_availableProfiles;
public:
	MimeTypeMap() {
		if (_availableProfiles.empty()) {
			_availableProfiles.insert("default");
			_availableProfiles.insert("cgi");
		}
	}
	static MimeMap& loadDefaultMimeMap();
	static MimeMap& loadMimeMap(const std::string& profile, const std::string& ext);
};



#endif //MIMETYPE_HPP
