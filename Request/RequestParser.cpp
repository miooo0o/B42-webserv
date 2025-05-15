/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 01:37:52 by kmooney           #+#    #+#             */
/*   Updated: 2025/05/15 17:59:07 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/RequestParser.hpp"
#include "includes/Request.hpp"

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>					CONSTRUCTION/DESTRUCTION/COPY					   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
																				*/
RequestParser::RequestParser(Request* request): _req(){
	_req = request;
	_req->setRstate(P_REQLINE);
	_rl_state = RL_LINE;
	_req->setPstate(P_PARSE);
	_unparsed_len = 0;
	_expected_len = 0;
}

RequestParser::~RequestParser(){}

/**
 * @brief Main entry point to parse raw HTTP request data.
 *
 * This function feeds raw HTTP data into the parser, initializes internal iterators,
 * and triggers the parsing process. It processes as much of the request as possible
 * given the available data. Any remaining unparsed data length is recorded in `_unparsed_len`.
 *
 * @param data A pointer to the raw HTTP request data buffer.
 * @param length The size of the data buffer in bytes.
 * @param request A reference to the `Request` object to populate during parsing.
 *
 * @see addData()
 * @see setIterators()
 * @see parseRequest()
 */
void	RequestParser::parse(const char* data, size_t length, Request& request){
	if (length == 0){
		_req->setState(P_ERROR, P_REQLINE, "Empty Request", 400);
		return;
	}
	addData(data, length);
	setIterators();
	parseRequest();
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>				 ADDING DATA AND SETTING ITERATORS					   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
																				*/
/**
 * @brief Appends raw data bytes to the internal parsing buffer.
 *
 * This function takes a pointer to a buffer and appends each byte to the internal `_buffer`
 * used by the parser. This buffer accumulates data across multiple calls until a full request
 * can be parsed.
 *
 * @param data Pointer to the raw input data buffer.
 * @param length Number of bytes to append from the input buffer.
 *
 * @note This function does not perform any parsing or validation — it only stores data.
 */
void	RequestParser::addData(const char* data, size_t length){
	for (std::size_t i = 0; i < length; i++)
		_buffer.push_back(data[i]);
}

/**
 * @brief Initializes internal parsing iterators for processing the request buffer.
 *
 * This function sets the beginning and end positions of the `_p_it` token range,
 * which is used to track the portion of the buffer currently being parsed.
 * 
 * If the request is in the `P_REQLINE` state, it resets the beginning of the parse range
 * to the start of the buffer. The end iterator is always set to the current end of the buffer.
 *
 * @note This function must be called after new data is added to the buffer via `addData()`.
 *
 * @see _p_it
 * @see _req
 */
void	RequestParser::setIterators(){
	if (_req->_r_state == P_REQLINE)
		_p_it.begin = _buffer.begin();
	_p_it.end = _buffer.end();
}

/**
 * @brief Sets the begin and end iterators of a token.
 *
 * This templated utility function assigns the provided `begin` and `end` iterators
 * to the corresponding fields of the given `token_t` object.
 *
 * @tparam T The iterator type (e.g., std::string::iterator or std::vector<char>::iterator).
 * @param t The token object whose iterator boundaries will be set.
 * @param begin The iterator pointing to the start of the token's range.
 * @param end The iterator pointing to the end of the token's range.
 *
 * @note This function is commonly used to isolate and define specific ranges within
 * the request buffer for parsing.
 */
template <class T>
void	RequestParser::setIterators(token_t& t, T& begin, T& end){
	t.begin = begin;
	t.end = end;
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>					 SPLITING FUNCTIONS								   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
																				*/																	
/**
 * @brief Extracts the next line from the input token stream.
 *
 * This function attempts to split the given token `it` at the next line delimiter (typically CRLF or LF),
 * storing the result in `line`. It is used during HTTP request parsing to extract individual lines
 * (such as the request line or header lines) from the raw input buffer.
 *
 * Internally, it delegates to `splitToToken()` to perform the actual line splitting.
 *
 * @param it The input token stream to split.
 * @param line A token that will receive the next line extracted from the input.
 * @return true if a line was successfully extracted; false if no full line is currently available.
 *
 * @see splitToToken()
 */
bool	RequestParser::parseNextLine(token_t& it, token_t& line) {
	return splitToToken(it, false, line, LINE);
}

/**
 * @brief Parses a single token from the input stream, splitting at a specified delimiter.
 *
 * This function extracts a single token from the input token stream `it` and stores it in
 * the `dest` string. The function splits the input based on the specified delimiter `delim` (e.g., a space, colon, etc.).
 * If the delimiter is not found or if the token cannot be parsed, it returns false, and if `delim_req` is true,
 * it sets the parser state to an error.
 *
 * @param it The input token stream to parse.
 * @param last_val Indicates whether this is the last token in the stream.
 * @param dest A reference to a string where the parsed token will be stored.
 * @param delim The delimiter to split the token at.
 * @param delim_req If true, an error state will be set if the delimiter is not found.
 * @return true if the token was successfully parsed and stored in `dest`, false if parsing failed.
 *
 * @see splitToToken()
 * @see tokenToString()
 * @see RequestParser::setPstate()
 */
bool	RequestParser::parseSingleToken( token_t& it, bool last_val, std::string& dest, const std::string& delim, bool delim_req ){
	token_t tkn;

	if (splitToToken(it, last_val, tkn, delim)){
		tokenToString(tkn, dest);
		return true;
	}
	if (delim_req == true)
		_req->setPstate(P_ERROR);
	return false;
}

/**
 * @brief Splits a token into a vector of tokens based on a string delimiter.
 * 			The delimiter is discarded.
 *
 * This function splits a token at the occurrence of a sequence of characters defined by the `delim` string.
 * The resulting tokens are stored in the `tkns` vector.
 *
 * If `last_val` is true, the remainder of the token is included as the last token.
 *
 * @param it The token to be split, represented by the `token_t` object.
 * @param last_val If true, the final portion of the token is included even if it is not followed by a delimiter.
 * @param tkns A vector to store the resulting tokens.
 * @param delim The string used as the delimiter for splitting.
 * @return The number of tokens generated.

 */

size_t	RequestParser::splitToToken( token_t& it, bool last_val, std::vector<token_t>& tkns, const std::string& delim )
{

	// NEED TO CHECK FOR BARE CR (\r WITHOUT \n)
	// NEED TO ADD DQUOTE CHECK AND ESCAPE CHAR
	std::deque<char>::const_iterator	parse_pos = it.begin;
	while (parse_pos != it.end)
	{
		std::deque<char>::const_iterator	compare = parse_pos;
		std::string::const_iterator			delim_pos = delim.begin();
		while (compare != it.end && delim_pos != delim.end() && *compare == *delim_pos) {
			compare++;
			delim_pos++;
		}
		if (delim_pos == delim.end()){
			tkns.push_back(token_t(it.begin, parse_pos));
			parse_pos = compare;
			it.begin = parse_pos;
		}
		else
			parse_pos++;
	}
	if (last_val == true && it.begin != it.end){
		tkns.push_back(token_t(it.begin, it.end));
		it.begin = it.end;
	}
	return tkns.size();
}

/**
 * @brief Finds and extracts a token before a string delimiter (delimiter is discarded)
 *
 * This function looks for the occurrence of a string delimiter in the token and extracts the portion
 * of the token that precedes it. The result is stored in `tkn`.
 *
 * If the delimiter is found, the iterator `it` is advanced to the character following the delimiter.
 *
 * @param it The token to search within, represented by the `token_t` object.
 * @param last_val If true, the last portion of the token is included even if it is not followed by a delimiter.
 * @param tkn A reference to the `token_t` object where the extracted token will be stored.
 * @param delim The string delimiter to search for.
 * @return true if a token was successfully extracted; false if no delimiter was found.
 */
bool	RequestParser::splitToToken( token_t& it, bool last_val, token_t& tkn, const std::string& delim )
{
	std::deque<char>::const_iterator	parse_pos	= it.begin;
	std::string::const_iterator 		delim_it	= delim.begin();
	
	while (parse_pos != it.end)
	{	
		std::deque<char>::const_iterator	compare = parse_pos;
		std::string::const_iterator			delim_pos = delim.begin();

		while (compare != it.end && delim_pos != delim.end() && *compare == *delim_pos) {
			compare++;
			delim_pos++;
		}
		if (delim_pos == delim.end()){
			setIterators(tkn, it.begin, parse_pos);
			it.begin = compare;
			return true;
		}
		else
			parse_pos++;
	}
	if (last_val == true && it.begin != it.end){
		setIterators(tkn, it.begin, it.end);
		return true;
	}
	return false;
}

bool	RequestParser::splitToTokenMultiDelims(token_t& it, bool last_val, token_t& tkn, const std::string& delims, char& found_delim) 
{
	std::deque<char>::const_iterator parse_pos = it.begin;

	while (parse_pos != it.end) 
	{
		if (delims.find(*parse_pos) != std::string::npos) {
			setIterators(tkn, it.begin, parse_pos);
			found_delim = *parse_pos;
			it.begin = parse_pos + 1;
			return true;
		}
		parse_pos++;
	}

	if (last_val && it.begin != it.end) {
		setIterators(tkn, it.begin, it.end);
		found_delim = '\0';
		it.begin = it.end;
		return true;
	}

	found_delim = '\0';
	return false;
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>					 HELPER FUNCTIONS		  						   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

/**
 * @brief Converts a token into a string.
 *
 * This function converts a `token_t` object, which contains a range defined by iterators (`begin` and `end`),
 * into a `std::string` by constructing the string from the characters between the `begin` and `end` iterators.
 *
 * @param token The token object to be converted to a string.
 * @return A `std::string` representing the characters between the `begin` and `end` iterators of the token.
 */
void	RequestParser::tokenToString(const token_t& token, std::string& dest) {
	dest.assign(token.begin, token.end);
}

std::string	RequestParser::tokenToString(const token_t& token, std::string& dest, int debug) {
	return std::string(token.begin, token.end);
}

/**
 * @brief Retrieves the length of the remaining unparsed data.
 *
 * This function returns the number of bytes of data that have not yet been parsed by the request parser.
 * This is useful for tracking the progress of data parsing and determining how much more data needs
 * to be processed.
 *
 * @return The number of unparsed bytes in the request buffer.
 */

size_t	RequestParser::getUnparsedLength(){
	_unparsed_len = std::distance(_p_it.begin, _p_it.end);
	return _unparsed_len;
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>					 PARSING CONTROL		  						   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

/**
 * @brief Parses an HTTP request by processing its various components: request line, headers, and body.
 * 
 * This function checks the current state of the request (`_r_state`) and invokes the appropriate parsing 
 * function based on the state. The states& include:
 * - `P_REQLINE`: Parses the request line (method, URI, and protocol).
 * - `P_HEADERS`: Parses the HTTP headers.
 * - `P_BODY`: Parses the body of the HTTP request.
 * 
 * The `_p_state` is set to `P_PARSE` to indicate that parsing is in progress.
 * 
 * @note The function works by calling the specific parsing functions like `parseReqLine`, `parseHeaders`, 
 * and `parseBody` depending on the state of the request.
 */
void	RequestParser::parseRequest(){

	_req->_p_state = P_PARSE;
	switch (_req->_r_state){
		case P_REQLINE	: { parseReqLine(_p_it); break; }
		case P_HEADERS	: { parseHeaders(_p_it); break; }
		case P_BODY		: { parseBody	(_p_it); break; }
		default			: { break; }
	}
	return ;
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>					 PARSING REQUEST LINE	  						   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

/**
 * @brief Parses the HTTP request line.
 * 
 * This function processes the request line (which includes the HTTP method, URI, and version) of 
 * the incoming HTTP request. It uses the `getRequestLine` function to retrieve the request line 
 * and then parses it in stages, checking each component (method, URI, version) in order. 
 * The parsing progresses through the following states&:
 * - `RL_METHOD`: Parses the HTTP method (e.g., GET, POST).
 * - `RL_URI`: Parses the requested URI.
 * - `RL_VERSION`: Parses the HTTP version (e.g., HTTP/1.1).
 * - `RL_COMPLETE`: Indicates that the request line has been fully parsed.
 * 
 * The function checks for any parsing errors, and if an error is detected, it sets the appropriate
 * error state and message.
 * 
 * @param it The token iterator pointing to the current position in the input data.
 * 
 * @return This function does not return any value but modifies the state of the request and 
 *         the `_rl_state` during parsing.
 * 
 * @note If the request line format is incorrect, the state is set to `P_ERROR` and an error message is logged.
 */
void	RequestParser::parseReqLine(token_t& it){

	token_t	rl;
	
	if (_rl_state == RL_LINE && !getRequestLine(rl, it))
		return;

	while (rl.begin != rl.end){
		switch(_rl_state){
			case RL_METHOD	: { parseSingleToken(rl, false, _req->_method, SPACE, DELIM_REQUIRED ); break; }
			case RL_URI		: { parseURI(rl); break; }
			case RL_VERSION	: {	parseSingleToken(rl, true, _req->_version, SPACE, DELIM_OPTIONAL ); break; }
			case RL_COMPLETE: {	_req->setRstate(V_REQLINE); return; }
			default			: { break; }
		}
		if (_req->getParseStatus() == P_ERROR){
			_req->setState(P_ERROR, P_REQLINE, "Request Line Format Error", 400);
			return;
		} 
		_rl_state++;
	}
	return;
}

/**
 * @brief Retrieves the request line from the HTTP request.
 * 
 * This function attempts to parse the next line from the input iterator and assigns it to the 
 * `rl` token. If the line is successfully parsed, it sets the request line state to `RL_METHOD`.
 * If the line cannot be parsed, the function sets the request parsing state to `P_AWAIT`.
 * 
 * @param rl The token where the parsed request line will be stored.
 * @param it The iterator pointing to the current position in the input data.
 * 
 * @return `true` if the request line was successfully parsed, `false` otherwise.
 */
bool	RequestParser::getRequestLine(token_t& rl, token_t &it){

	if (!parseNextLine(it, rl)) {
		_req->setPstate(P_AWAIT);
		return false;
	}
	else
		_rl_state = RL_METHOD;
	return true;
}


/*
>>>>>>>>>>>>>>>>>>>>>><<<<<<
>>>>>>	 URI PARSING  <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>><<<<
*/
bool	RequestParser::parseURI( token_t& rl )
{
	token_t	uri;
	states	state = SCHEME;
	
	if (!splitToToken(rl, false, uri, SPACE ))
		return _req->setState(P_ERROR, P_REQLINE, "Request Line Format Error", 400);

	tokenToString(uri, _req->_uri_str);
	while (uri.begin != uri.end)
	{
		switch (state) {
			case SCHEME:parseScheme(state, uri); break;
			case USER:	parseUser(state, uri); break;
			case HOST: 	parseHost(state, uri); break;
			case PORT: 	parsePort(state, uri); break;
			case PATH: 	parsePath(state, uri); break;
			case QUERY: parseQuery(state, uri); break;
			case FRAG:	parseFrag(state, uri); break;
			case URI_END: return true;
			default: break;
		}
		if (_req->_p_state == P_ERROR)
			return false;
	}
	return true;
}

void	RequestParser::parseScheme( states& state, token_t& uri ){
	if (parseSingleToken(uri, false, _req->_uri.scheme, "://", DELIM_OPTIONAL)){
		state = USER;
		_req->_uri.uri_type = ABSOLUTE_FORM;
	}
	else if (parseSingleToken(uri, false, _req->_uri.scheme, FSLASH, DELIM_OPTIONAL)){
		state = PATH;
		_req->_uri.path.append("/");
	}
	else
		_req->setState(P_ERROR, P_REQLINE, "URI error - no path, or authority", 400);
}

void	RequestParser::parseUser( states& state, token_t& uri ){

	token_t userinfo;

	if (splitToToken(uri, false, userinfo, "@" )){
		if (parseSingleToken(userinfo, true, _req->_uri.user, ":", DELIM_OPTIONAL))
			parseSingleToken(userinfo, true, _req->_uri.pass, "", DELIM_OPTIONAL);
	}
	state = HOST;
	return;
}

void	RequestParser::parseHost( states& state, token_t& uri ){

	token_t		host;
	char		found_delim;
	
	if (splitToTokenMultiDelims(uri, false, host, ":/?#", found_delim )){
		tokenToString(host, _req->_uri.host);
		switch (found_delim){
			case ':' : { state = PORT; return; }
			case '/' : { state = PATH; return; }
			case '?' : { state = QUERY; return; }
			case '#' : { state = FRAG; return; }
			default	 : { break;}
		}
	}
	state = URI_END;
}

void	RequestParser::parsePort( states& state, token_t& uri ){
	token_t		port;
	char		found_delim;
	
	if (splitToTokenMultiDelims(uri, true, port, "/?#", found_delim )){
		tokenToString(port, _req->_uri.port);
		_req->_uri.uri_type = AUTH_FORM;
		switch (found_delim){
			case '/' : { state = PATH; _req->_uri.path.append("/"); return; }
			case '?' : { state = QUERY; return; }
			case '#' : { state = FRAG; return; }
			default	 : { break;}
		}
	}
	state = URI_END;
}

void	RequestParser::parsePath( states& state, token_t& uri ){
	token_t		path;
	char		found_delim;
	
	if (splitToTokenMultiDelims(uri, true, path, "?#", found_delim )){
		tokenToString(path, _req->_uri.path);
		switch (found_delim){
			case '?' : { state = QUERY; return; }
			case '#' : { state = FRAG; return; }
			default	 : { break;}
		}
	}
	state = URI_END;
}

/*  URI QUERY PARSE  */

void	RequestParser::parseQuery( states& state, token_t& uri ){
/*
	fragment    = *( pchar / "/" / "?" )
	pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
	Locate the ? in the URI to extract the query part.
	Split the query string at & to separate key-value pairs.
	Split each pair at = to extract the key and value.
	Perform URL decoding if necessary (e.g., %20 → space, + → space).

	unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
  	
	reserved      = gen-delims / sub-delims
  	
	gen-delims    = ":" / "/" / "?" / "#" / "[" / "]" / "@"

  	sub-delims    = "!" / "$" / "&" / "'" / "(" / ")"
                 / "*" / "+" / "," / ";" / "="
*/	
	token_t		query;

	if (splitToToken(uri, false, query, "#")){
		tokenToString(query, _req->_uri.query);
		state = FRAG;
	}
	else
		state = URI_END;
	
	std::string	key;
	std::string	val;
	std::vector<token_t> querylist;

	if (state == URI_END)
		size_t size = splitToToken(uri, true, querylist, "&");
	else
		size_t size = splitToToken(query, true, querylist, "&");
	std::vector<token_t>::iterator it = querylist.begin();

	for (; it != querylist.end(); it++) {
		if (parseSingleToken(*it, true, key, "=", DELIM_REQUIRED)) {
			tokenToString(*it, val);
			to_lower_ref(key);
			_req->_uri.queries.push_back(std::make_pair(key, val));
		}
	}
}

/*  URI FRAGMENT PARSING  */

void	RequestParser::parseFrag( states& state, token_t& uri ){
	tokenToString(uri, _req->_uri.frag);
	state = URI_END;
}

bool	RequestParser::URInormalisation( void ){
	return true;
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>					 PARSING HEADERS		  						   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

/**
 * @brief Parses the value of an HTTP header field, extracting the main value and its parameters.
 *
 * This function takes a header field value token (e.g., `"text/html; charset=UTF-8"`),
 * splits it by semicolons, and stores the first token as the main value.
 * Remaining tokens are treated as optional key-value parameters and stored in the `params` list
 * of the `HeaderField_t` object.
 *
 * Each optional parameter is parsed as a key-value pair (e.g., `"charset=UTF-8"`).
 * Keys are converted to lowercase for normalization.
 *
 * @param value_tkn The token representing the raw value of the header field (possibly with parameters).
 * @param header_field A pointer to a `HeaderField_t` struct that will be populated with the parsed data.
 * @return true Always returns true (parsing is assumed successful).
 *
 * @see splitToToken()
 * @see parseSingleToken()
 * @see tokenToString()
 * @see to_lower_ref()
 */
bool	RequestParser::parseHeaderVal(token_t& value_tkn, HeaderField_t* header_field) {
	
	std::vector<token_t>			options_vector;
	std::vector<token_t>::iterator	it;
	
	token_t token;
	std::string	option_key;
	std::string	option_val;

	size_t options = splitToToken(value_tkn, true, options_vector, ";");
	{	
		it = options_vector.begin();
		tokenToString(*it++, header_field->val);
		trimWS(header_field->val);

		for (; it != options_vector.end(); it++) {
			if (parseSingleToken(*it, true, option_key, "=", DELIM_REQUIRED)) {
				tokenToString(*it, option_val);
				trimWS(option_val);
				trimWS(option_key);
				to_lower_ref(option_key);
				header_field->params.push_back(std::make_pair(option_key, option_val));
			}
		}
	}
	return true;
}

/**
 * @brief Parses a single HTTP header line and stores the result in the request object.
 *
 * This method processes a token representing an HTTP header line (e.g., "Content-Type: text/html").
 * It extracts the header field name and associated values, normalizes the name to lowercase,
 * splits comma-separated values if present, and stores them as a vector of `HeaderField_t*`
 * in the `_req->_headers` map.
 *
 * If the line is malformed (e.g., missing a colon delimiter), the method sets the request state
 * to an error with HTTP status code 400.
 *
 * @param line The tokenized line representing one HTTP header line.
 * @return true if the header line was successfully parsed and stored; false if the line is malformed.
 *
 * @see parseSingleToken()
 * @see parseHeader_values()
 * @see RequestParser::setState()
 */

bool	RequestParser::parseHeaderLine(token_t& line) {
	std::string						header_name;
	std::vector< HeaderField_t * >	header_field_vector;

	if (parseSingleToken(line, false, header_name, ":", DELIM_REQUIRED)) {
		to_lower_ref(header_name);

		std::vector<token_t> values;
		int	size = splitToToken(line, true, values, ",");
	
		for (size_t i = 0; i < values.size(); i++) {
			token_t& currentToken = values[i];
	
			HeaderField_t* header_field = new HeaderField_t();
			parseHeaderVal(currentToken, header_field);
			header_field_vector.push_back(header_field);
		}
		_req->_headers[header_name] = header_field_vector;
		return true;
	}
	else {
		_req->setState(P_ERROR, P_HEADERS , "Malformed header field : no field name", 400);
		return false;
	}
}

/**
 * @brief Parses all HTTP header lines from the input token stream.
 *
 * This function iteratively extracts and parses header lines from the provided token stream `it`.
 * For each line:
 * - If the line is empty, it signals the end of the headers section and transitions the request
 *   to the `V_HEADERS` state.
 * - If the line cannot be read (e.g., due to incomplete input), it sets the parser state to `P_AWAIT`
 *   to await more data.
 * - If a header line is malformed, parsing is aborted (the error is set by `parseHeaderLine()`).
 *
 * On successful parsing of all headers, the parser state is set to `P_AWAIT`.
 *
 * @param it The token stream representing the raw HTTP header section to be parsed.
 *
 * @see parseNextLine()
 * @see parseHeaderLine()
 * @see RequestParser::setPstate()
 * @see RequestParser::setRstate()
 */
void	RequestParser::parseHeaders(token_t& it) {
	token_t line;

	while (it.begin != it.end) {
		if (!parseNextLine(it, line)) {
			_req->setPstate(P_AWAIT);
			return;
		}
		if (line.begin == line.end) {
			_req->setRstate(V_HEADERS);
			return;
		}
		if (!parseHeaderLine(line)) {
			return;
		}
	}
	_req->setPstate(P_AWAIT);
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>						 PARSING BODY		  						   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

/* FUNCTION TO PARSE THE BODY */
void	RequestParser::parseBody(token_t& it) {
	token_t line;

	while (it.begin != it.end){
		if (_req->_isChunked){
			parseChunked(it);
			return;
		}
		else if (_req->_isMultiPart){
			parseMultiPart(it);
			return;
		}
		else{
			parsePlain(it);
			return;
		}
	}
	_req->setPstate(P_AWAIT);
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>						 PARSING PLAIN BODY	  						   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/
void	RequestParser::parsePlain(token_t& it) {

	size_t remaining = _req->_content_length - _req->_body.length();
	size_t available = std::distance(it.begin, it.end);
	size_t to_copy   = std::min(remaining, available);

	if (to_copy > 0) {
		_req->_body.append(it.begin, it.begin + to_copy);
		it.begin += to_copy;
	}
	if (_req->_body.length() < _req->_content_length) {
		_req->setPstate(P_AWAIT);
		return;
	}
	if (_req->_body.length() != _req->_content_length)
		_req->setState(P_ERROR, P_REQLINE, "Body size does not match length", 400);
	else
		_req->setState(P_DONE, V_BODY, "Parsing Complete", 200);
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>					 PARSING CHUNKED BODY							   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

bool	hasBytesAvailable(token_t& it, size_t required) {
	return std::distance(it.begin, it.end) >= static_cast<ptrdiff_t>(required);
}

bool	RequestParser::parseChunkData(token_t& it, std::string &data) {

	size_t	total_needed = _chunk_size + 2;

	if (!hasBytesAvailable(it, total_needed)) {
		_req->setPstate(P_AWAIT);
		return false;
	}

	data.assign(it.begin, it.begin + _chunk_size);
	it.begin += _chunk_size;

	if (*it.begin != '\r' || *(it.begin + 1) != '\n') {
		_req->setState(P_ERROR, P_REQLINE, "Invalid chunk termination", 400);
		return false;
	}
	it.begin += 2;
	return true;
}

bool	RequestParser::parseChunkSize( token_t& it){
	token_t	line;
	
	if (!parseNextLine(it, line)) {
		_req->setPstate(P_AWAIT);
		return false;
	}

	std::string	size_str;

	parseSingleToken(line, true, size_str, ";", DELIM_OPTIONAL);
	if (substrToHex(_chunk_size, size_str, 0, size_str.length()))
		return true;
	return false;
}

void	RequestParser::parseChunked( token_t& it ){

	std::string		data;
	token_t			test_empty;

	while (it.begin != it.end) {
		if (_chunk_size == 0 && !parseChunkSize(it)){
			_req->setState(P_ERROR, P_BODY, "Invalid chunk size", 400);
			return;
		}
		if (_chunk_size == 0){
			if (parseNextLine(it, test_empty) && test_empty.begin == test_empty.end){
				_req->setRstate(V_BODY);
				_req->setPstate(P_DONE);
				return;
			}
			else
				_req->setState(P_ERROR, P_BODY, "Invalid Characters at end of chunking", 400);
		}
		if (!parseChunkData(it, data))
			return;
		_req->_body += data;
		_chunk_size = 0;
	}
}

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>					 PARSING MULTIPART								   <<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

void	RequestParser::parseMultiPart( token_t& it ){

// 	std::string		data;

// 	while (it.begin != it.end) {
// 		if (_chunk_size == 0 && !parseChunkSize(it))
// 			return;
// 		if (_chunk_size == 0){
// 			_req->setRstate(V_BODY);
// 			_req->setPstate(P_DONE);
// 			return;
// 		}
// 		if (!parseChunkData(it, data))
// 			return;
// 		_req->_body += data;
// 		_chunk_size = 0;
// 	}
}
