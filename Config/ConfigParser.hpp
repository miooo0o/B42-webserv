#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <istream>
#include <iostream>
#include <vector>
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
private:
	std::istream&							_input;
	std::ostream*	_log;
	int				_status;
public:
	ConfigParser(std::istream& input)
		: _input(input), _log(NULL), _status(-1) {};

	std::vector<ServerConfig>	parse();
	
	// getter
	std::ostream&				getLog() { return _log}
	int							getStatus() { return _status }

private:
	void						removeComment();
	void						splitServerBlocks();
	std::vector<std::istream>	toEachServerBlock();
	std::vector<std::istream>	toEachLocationBlock();
	
	void	logging(std::string& msg);
};

std::ostream& operator<<(std::ostream& os, const ConfigParser& parser);

#endif //CONFIGPARSER_HPP
