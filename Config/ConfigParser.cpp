#include "ConfigParser.hpp"

#include <sstream>

#include "ServerConfigBase.hpp"


// ============================================================================================================
// [public]
// ============================================================================================================

ConfigParser::ConfigParser(std::istream& input)
	: _input(input), _log(NULL) {};


std::vector<ServerConfigBase> ConfigParser::parse() {
	std::string line;
	std::vector<ServerConfigBase> configBases;
	std::string leftover;
	static int	serverBlockCounter = 1;
	while (std::getline(_input, line)) {
		mergeLeftover(line, leftover);
		removeCommentAndTrim(line);
		if (line.empty()) continue;

		try {
			server_logging(serverBlockCounter, "Parsing process start");
			ParseResult result = parseLineAsServerBlock(line);
			configBases.push_back(result.base);
			leftover = result.leftover;
			server_logging(serverBlockCounter++, "Successfully parsed block");
		} catch (const ServerBlockSkipException& e) {
			server_logging(serverBlockCounter++, "Skipping block: " + std::string(e.what()));
		} catch (const FatalConfigException& e) {
			server_logging(serverBlockCounter++, "Fatal: " + std::string(e.what()));
			throw; // throw FatalConfigException?
		}
	}
	return configBases;
}


// ============================================================================================================
// [private]
// ============================================================================================================

void ConfigParser::mergeLeftover(std::string& line, const std::string& leftover) {
	if (!leftover.empty()) {
		std::string trimmed = leftover;
		trim(trimmed);
		if (!trimmed.empty()) {
			line = trimmed + " " + line;
		}
	}
}

ParseResult ConfigParser::parseLineAsServerBlock(std::string& line) {
	ConfigTokens tokens = tokenizer(line);
	if (tokens.keyword != "server")
		throw FatalConfigException("Unexpected content outside of server block");

	parseServerBlock(line, tokens);
	return ParseResult { ServerConfigBase(tokens), leftover };
}

void	ConfigParser::parseServerBlock(std::string& line, ConfigTokens& block) {
	while (getline(_input, line)) {
		removeCommentAndTrim(line);
		if (line == "}")
			return ;
		if (startsWith(line, "location")) {
			parseLocationBlock(line);
		} else {
			parseDirective(line);
		}
	}
}

ConfigTokens ConfigParser::tokenizer(std::string& line) {
	const char		symbol = readUntilBlockOpensOrSemicolon(line);
	ConfigTokens	tokens;
	const size_t	symbol_pos = (symbol == '{') ? line.find('{') : line.find(';');

	if (symbol_pos == std::string::npos)
		throw FatalConfigException("Line must end with '{' or ';'");

	std::string beforeSymbol = line.substr(0, symbol_pos);
	if (symbol_pos + 1 < line.size()) {
		const std::string afterSymbol = line.substr(symbol_pos + 1);
		mergeLeftover(line, afterSymbol);
	} else {
		line = "";
	}

	std::vector<std::string> words = splitIntoTokens(beforeSymbol);
	if (words.empty())
		throw FatalConfigException("Missing keyword");

	tokens.keyword = words[0];

	// todo: refactor -> small function
	std::vector<std::string>::iterator it = words.begin();
	++it;
	for (; it != words.end(); ++it)
		tokens.args.push_back(*it);

	tokens.isBlock = (symbol == '{');

	return tokens;
}



char	ConfigParser::readUntilBlockOpensOrSemicolon(std::string& line) {
	std::string	buffer;
	char		found = '\0';

	while (true) {
		removeCommentAndTrim(line);
		if (!line.empty()) {
			if (!buffer.empty())
				buffer += " ";
			buffer += line;

			const size_t bracePos = line.find('{');
			const size_t semiPos = line.find(';');

			if (bracePos != std::string::npos || semiPos != std::string::npos) {
				if (bracePos != std::string::npos &&
					(semiPos == std::string::npos || bracePos < semiPos)) {
					found = '{';
					} else {
						found = ';';
					}
				break;
			}
		}
		if (!std::getline(_input, line))
			throw FatalConfigException("Unexpected EOF while looking for '{' or ';'");
	}
	line = buffer;
	return found;
}

void    ConfigParser::removeCommentAndTrim(std::string& line) {
	const size_t	commentPos = line.find('#');
	if (commentPos != std::string::npos)
		line = line.substr(0, commentPos);
	trim(line);
}

void	ConfigParser::trim(std::string& line) {
	// todo: implement
}

ConfigTokens ConfigParser::extractKeywordFromLine(std::string& line) {
	std::istringstream	iss(line);
	std::string			keyword;
	ConfigTokens		tokens;

	iss >> keyword;
	if (keyword.empty())
		throw FatalConfigException("Missing keyword in line");
	const size_t keyword_pos = line.find(keyword);

	if (keyword_pos != std::string::npos) {
		mergeLeftover(line, line.substr(keyword_pos + keyword.length()));
	}
	tokens.keyword = keyword;
	return tokens;
}

// logging (optional)
void    ConfigParser::logging(const std::string& msg) const {
	static int index = 0;
	*_log << "[" << index++ << "]"<< msg << '\n';
}

void    ConfigParser::server_logging(const int index, const std::string& msg) const {
	*_log << "[server " << index << "]"<< msg << '\n';
}

void    ConfigParser::location_logging(const int index, const std::string& msg) const {
	*_log << "[server " << index << "]"<< msg << '\n';
}


/*

// std::string	extractBlockHeaderAndInline(const std::string& line, ConfigTokens& block);
// void		validateBraceTokens(const ConfigTokens& block);

std::string	ConfigParser::extractBlockHeaderAndInline(const std::string& line, Block& block) {
	const size_t bracePos = line.find('{');
	if (bracePos == std::string::npos)
		throw FatalConfigException("Missing '{' in block header");

	const size_t braceCount = static_cast<size_t>(std::count(line.begin(), line.end(), '{'));
	if (braceCount != 1)
		throw FatalConfigException("Too many '{' in block declaration");

	std::string beforeBrace = line.substr(block.token_last_index_pos, bracePos - block.token_last_index_pos);
	std::string afterBrace = line.substr(bracePos + 1);

	removeCommentAndTrim(beforeBrace);
	removeCommentAndTrim(afterBrace);
	block.path = beforeBrace;
	block.leftover = afterBrace;
	return afterBrace;
}

void ConfigParser::validateBraceTokens(const Block& block) {
	if (block.token == "server" && !block.valid())
		throw FatalConfigException("Unexpected tokens before '{' in server block");
	if (block.token == "location" && !block.valid())
		throw FatalConfigException("Unexpected tokens before '{' in location block");
}
*/

