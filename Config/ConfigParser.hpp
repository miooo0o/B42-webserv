#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <istream>

class ServerConfig;

/*
Config
 └── ServerBlock*
	  ├── Listen
	  ├── Root
	  ├── ServerName		(optional)
	  ├── Index				(optional)
	  ├── ErrorPages		(optional)
	  ├── ClientLimit		(optional)
	  ├── AllowMethods		(optional)
	  ├── Return			(optional)
	  ├── LocationBlock*	(optional)
	  └── Include			(optional)

LocationBlock
 ├── path
 ├── root					(optional)
 ├── allow_methods			(optional)
 ├── autoindex				(optional)
*/

class ConfigParser {
public:
	ConfigParser(std::istream& input);
	std::vector<ServerConfig> parse();

};



#endif //CONFIGPARSER_H
