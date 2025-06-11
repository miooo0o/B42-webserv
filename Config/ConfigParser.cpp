#include "ConfigParser.hpp"
#include "ServerConfigBase.hpp"

// ============================================================================================================
// [public]
// ============================================================================================================

ConfigParser::ConfigParser(std::istream& input)
	: _input(input), _log(NULL), _status(-1) {};

std::vector<ServerConfigBase>   ConfigParser::parse() {

}

// ============================================================================================================
// [private]
// ============================================================================================================

void    ConfigParser::removeComment(std::string& line) {
	while () {

	}
}


// logging (optional)
void    ConfigParser::logging(std::string& msg) {
	static int index = 0;
	_log << "[" << index++ << "]"<< msg << '\n';
}

// operator <<
std::ostream& operator<<(std::ostream& os, const ConfigParser& parser) {
	if (parser.getLog()) {
		os << "parse\n" << parser.getLog();
		os << parser.getStatus();
	}
	else {
		os << "parse\n" << "empty, \'"<< parser.getStatus() >> "\'";
	}
	return os;
}
