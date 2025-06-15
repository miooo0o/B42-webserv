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
	ServerConfigBase	base;
	std::string			leftover;

	ParseResult() : leftover("") {}
	ParseResult(const ServerConfigBase & base, const std::string& leftover)
		: base(base), leftover(leftover) {}

	bool	hasStash() { return !leftover.empty(); }
	void	clear() { base.clear(); leftover.clear(); }
};

class ConfigParser {
	enum Status {
		SET_NON = -1,
		FAILED = 0,
		SUCCESS = 1
	};
private:
	std::istream&		_input;
	std::ostream*		_log;\
public:
	ConfigParser(std::istream& input);
	std::vector<ServerConfigBase>	parse();
	
	// Getter
	std::ostream&				getLog() const { return *_log; }

private:

	ConfigTokens	tokenizer(std::string& line);
	// parse server block
	ParseResult	parseServerBlock(std::string &line, ConfigTokens& block);
	void		parseBlockHeaderLine(std::string &currentLine, ConfigTokens& block);

	char		readUntilBlockOpensOrSemicolon(std::string& currentLine);
	ParseResult	parseLineAsServerBlock(std::string& line);
	void mergeLeftover(std::string &line, const std::string &leftover);

	// utils
	void	removeCommentAndTrim(std::string &line);
	void	trim(std::string& line);
	ConfigTokens	extractKeywordFromLine(std::string &line);


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
