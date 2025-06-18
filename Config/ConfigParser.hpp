#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <iostream>
#include <istream>
#include <vector>

#include "ServerConfigBase.hpp"

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

struct ConfigTokens {
	std::string					keyword;	// "server", "listen", "return", etc.
	std::vector<std::string>	args;		// tokens AFTER keyword
	bool						isBlock;    // true if this is a block (has '{')

	bool isBlockStart() const { return isBlock; }
	std::string toString() const;
};

struct ParseResult {
	std::string					type;
	std::vector<ParseResult>	children;
	std::vector<ConfigTokens>	directives;
	std::string					leftover;

	explicit ParseResult(const std::string& type): type(type) {}


	bool	hasLeftover() const { return !leftover.empty(); }
	void	clear() { leftover.clear(); }
};

class ConfigParser {
private:
	std::istream&		_input;
	std::ostream*		_log;\
public:
	ConfigParser(std::istream& input);
	std::vector<ServerConfigBase>	parse();
	
	// Getter
	std::ostream&					getLog() const { return *_log; }

private:

	ConfigTokens	tokenizer(std::string& line);
	void			extractArgs(std::vector<std::string>& words, ConfigTokens& tokens);


	// parse server block
	ParseResult		parseBlockOrThrow(std::string& line, const std::string& blockKeyword);
	ParseResult		buildServerConfig(const std::vector<ConfigTokens>& directives,
										 const std::vector<ConfigTokens>& locations, std::string& line);

	char			readUntilBlockOpensOrSemicolon(std::string& line);
	char			chooseCloserSymbol(const std::string& line);
	ParseResult		parseLineAsServerBlockOrThrow(std::string& line);
	void			mergeLeftover(std::string &line, const std::string &leftover);

	// utils
	std::string	cleanedLineFrom(std::string& line);
	void		cleanLine(std::string& line);


	void	trim(std::string& line);
	ConfigTokens	extractKeywordFromLine(std::string &line);
	bool			startsWith(const std::string& line, const std::string& target);


public:
	void	logging(const std::string& msg) const;
	void    server_logging(int index, const std::string& msg) const;
	void    location_logging(int index, const std::string& msg) const;

	class FatalConfigException : public std::runtime_error {
	public:
		explicit FatalConfigException(const std::string& msg)
			: std::runtime_error("Fatal Error: " + msg) {}
	};

	class ServerBlockSkipException : public std::runtime_error {
	public:
		explicit ServerBlockSkipException(const std::string& msg)
			: std::runtime_error("Skip ServerBlock: " + msg) {}
	};
};

#endif //CONFIGPARSER_HPP
