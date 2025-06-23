#include "ConfigParser.hpp"

#include <sstream>

#include "ServerConfigBase.hpp"


// ============================================================================================================
// [public]
// ============================================================================================================

ConfigParser::ConfigParser(std::istream& input)
	: _input(input), _log(NULL) { parse(); };


std::vector<ServerConfigBase> ConfigParser::parse() {
	std::string line;
	std::vector<ServerConfigBase> configBases;
	std::string leftover;
	static int	serverBlockCounter = 1;
	while (std::getline(_input, line)) {
		mergeLeftover(line, leftover);
		cleanLine(line);
		if (line.empty()) continue;

		try {
			server_logging(serverBlockCounter, "Parsing process start");
			ParseResult result = parseLineAsServerBlockOrThrow(line);
			configBases.push_back(ServerConfigBase(result));
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

ParseResult ConfigParser::parseLineAsServerBlockOrThrow(std::string& line) {
	const ConfigTokens token = tokenizer(line);
	if (token.keyword != "server")
		throw FatalConfigException("Expected server block");
	if (!token.isBlock)
		throw FatalConfigException("Expected server block with '{'");

	return parseBlockOrThrow(line, "server");
}

ParseResult ConfigParser::parseBlockOrThrow(std::string& line, const std::string& blockKeyword) {
	ParseResult result(blockKeyword);
	while (std::getline(_input, line)) {
		cleanLine(line);
		if (line == "}") { /* todo: trim line; */ break; }
		ConfigTokens token = tokenizer(line);
		if (token.keyword == "location") {
			ParseResult inner = parseBlockOrThrow(line, "location");
			inner.directives.insert(inner.directives.begin(), token);
			result.children.push_back(inner);
		} else {
			result.directives.push_back(token);
		}
	}
	return result;
}

ParseResult	ConfigParser::buildParseResult(ParseResult& result, std::string& line) {
	// trim line utill '}', if something left after '}', save '}'after to line.
	if (!line.empty()) { result.leftover = line; }
	if ()

	return result;
}

ConfigTokens ConfigParser::tokenizer(std::string& line) {
	const char		symbol = readUntilBlockOpensOrSemicolon(line);
	ConfigTokens	tokens;
	const size_t	symbol_pos = (symbol == '{') ? line.find('{') : line.find(';');

	if (symbol_pos == std::string::npos)
		throw FatalConfigException("Line must end with '{' or ';'");

	// todo: edge case, #으로 comment out이 섞여있을 경우
	std::string	before_comment_out = line;
	line.clear();
	while (before_comment_out.find('#') != std::string::npos) {
		line += cleanedLineFrom(before_comment_out);
	}
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
	extractArgs(words, tokens);
	tokens.isBlock = (symbol == '{');

	return tokens;
}

void	ConfigParser::extractArgs(std::vector<std::string>& words, ConfigTokens& tokens) {
	std::vector<std::string>::iterator it = words.begin();
	++it;
	for (; it != words.end(); ++it)
		tokens.args.push_back(*it);
}


/* edge case
 *	keyword correct-input; unexpected token left here
 *
 */
char	ConfigParser::readUntilBlockOpensOrSemicolon(std::string& line) {
	std::string	buffer;
	char		found = '\0';

	while (true) {
		cleanLine(line);
		if (!line.empty()) {
			if (!buffer.empty() && buffer.back() == '\n')
				buffer += " ";
			buffer += line;
		}
		found = chooseCloserSymbol(line);
		if (found != '\0')
			break;
		if (!std::getline(_input, line))
			throw FatalConfigException("Unexpected EOF while looking for '{' or ';'");
	}
	line = buffer;
	return found;
}

char ConfigParser::chooseCloserSymbol(const std::string& line) {
	const size_t bracePos = line.find('{');
	const size_t semiPos = line.find(';');

	if (bracePos != std::string::npos &&
		(semiPos == std::string::npos || bracePos < semiPos))
		return '{';
	if (semiPos != std::string::npos)
		return ';';
	return '\0';
}

std::string ConfigParser::cleanedLineFrom(std::string& line) {
	const size_t commentPos = line.find('#');
	if (commentPos == std::string::npos) {
		std::string trimmed = line;
		trim(trimmed);
		line.clear();
		return trimmed;
	}

	std::string before = line.substr(0, commentPos);
	std::string after = line.substr(commentPos + 1);
	trim(before);

	const size_t newlinePos = after.find('\n'); {}
	if (newlinePos != std::string::npos) {
		after = after.substr(newlinePos + 1);
		trim(after);
	}
	else
		after.clear();
	line = after;
	return before;
}

void    ConfigParser::cleanLine(std::string& line) {
	const size_t	commentPos = line.find('#');
	if (commentPos != std::string::npos)
		line = line.substr(0, commentPos);
	trim(line);
}


// todo: from here
void	ConfigParser::trim(std::string& line) {
	if (line.empty())
		return ;
}

bool ConfigParser::startsWith(const std::string& line, const std::string& target) {
	std::istringstream	iss(line);
	std::string			keyword;
	iss >> keyword;
	trim(keyword);
	return (keyword == target);
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