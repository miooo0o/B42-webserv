/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:04:15 by kmooney           #+#    #+#             */
/*   Updated: 2025/04/07 13:44:29 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"

/*	DEFAULT CONSTRUCTOR	
	PARSE REQUEST LINE MUST THEN BE CALLED WITH REQUEST LINE
	CAN USE THE RETURN VALUE PARSE REQUEST LINE (BOOL) TO DETERMINE IF ERROR OR NOT
	- I PREFER NOT TO USE THIS APPROACH
*/
Request::Request() :  _request_line(""), _header_line(""), _body(""), _method(), _version(), _uri(), 
	_headers(), _last_response_code(200), _error(0) {
}

/*	PARAMETRISED CONSTRUCTOR - TAKES REQUEST LINE AS PARAMETER
	
	PARSE REQUEST MUST THEN CALLED WITHOUT REQUEST LINE (OR COULD CALL PARSE)
	CAN CHECK REQUEST OBJECT FOR ERROR
*/
Request::Request( const str_t& str ) : _request_line(str), _header_line(""), _body(""), _method(), _version(), _uri(),
	_headers(), _last_response_code(200), _error(0) {
	parseRequestLine();
}

/*	PARAMETRISED CONSTRUCTOR - TAKES REQUEST LINE AND CONFIG AS PARAMETERS
*/
Request::Request( const str_t& str, Config* config ) : _request_line(str), _header_line(""), _body(""), _method(), _version(), _uri(),
	_headers(), _last_response_code(200), _error(0), _config(config) {
	parseRequestLine();
}

Request::~Request( void ) {}

/* NOT SURE THIS IS NECESSARY - WE COULD MAKE PRIVATE */
Request::Request( const Request& other ) : _request_line(other._request_line),_header_line(other._header_line),_body(other._body), 
	_method(other._method), _version(other._version), _uri(other._uri), _headers(other._headers),
	  _last_response_code(other._last_response_code), _error(other._error) {
}

/* NOT SURE THIS IS NECESSARY - WE COULD MAKE PRIVATE */
Request& Request::operator=( const Request& other ) {	
	if ( this != &other )
		*this = other;
	return ( *this );
}

  /* ========= */
 /*  PARSING  */
/* ========= */	

/* PARSE REQUEST LINE - PARAMETERISED */
/* 
	TAKES REQUEST LINE AS INPUT 
	I PREFER NOT TO USE THIS APPROACH, BUT CAN BE USED WITH IF STATEMENT IN CALLING FUNCTION
*/
bool	Request::parseRequestLine(const str_t& str)
{
	std::istringstream stream(str);

	bool outcome = true;
	
	if (!(parseMethod(stream) && parseURI(stream) && parseVersion(stream)))
		outcome = false;
	_route = setRoute();
	if (!( validateMethod() && validateURI() && validateVersion()))
		outcome = false;
	return outcome;
}

/* PARSE REQUEST LINE - NO PARAMETER */
/* 
NO PARAMETER - USES _request_line WHICH HAS BEEN SET BY PARAMETERISED CONSTRUCTOR
	CALLED FROM DEFAULT CONSTRUCTOR
*/
bool	Request::parseRequestLine()
{
	std::istringstream	stream(_request_line);
	bool				outcome = true;
	
	if (!(parseMethod(stream) && parseURI(stream) && parseVersion(stream)))
		outcome = false;

	_route = setRoute();

	if (!( validateMethod() && validateURI() && validateVersion()))
		outcome = false;
	return outcome;
}

/*  METHOD PARSING  */

bool	Request::parseMethod(std::istringstream& stream)
{
	std::getline(stream, _method, ' ');
	if (_method.empty())
		return false;
	return true;
}

/*  VERSION PARSING  */

bool	Request::parseVersion(std::istringstream& stream)
{
	std::getline(stream, _version, '\r');
	char ch;

    if (stream.get(ch) && ch == '\n') {
		if (!_version.empty())
			return true;
		return _error.addErrorFlag(errFlag::VERSION_NONE);
	}
	else
		return _error.addErrorFlag(errFlag::REQUEST_ENDING);
}

   /* ============= */
  /*  URI PARSING  */
 /* ============= */

bool	Request::parseURI(std::istringstream& stream)
{
	str_t	str;
	states	state = SCHEME;
	size_t	i = 0;

	std::getline(stream, _uri.str, ' ');
	if (_uri.str.empty())
		return false;
	
	_uri.len = _uri.str.length();
	
	while (i < _uri.len)
	{
		switch (state) {
			case AUTH:{
				if ( _uri.user.empty()) {parseURIState(state, _uri.user, i); }
				else { parseURIState(state, _uri.pass, i); }
				break;
			}
			case FRAG:	parseURIState(state, _uri.frag, i); break;
			case HOST: 	parseURIState(state, _uri.host, i);	break;
			case PATH: 	parseURIState(state, _uri.path, i);	break;
			case PORT: 	parseURIState(state, _uri.port, i);	break;
			case QUERY: parseURIState(state, _uri.query, i); break;
			case SCHEME:parseURIState(state, _uri.scheme, i); break;
			default: break;
		}
	}
	return true;
}

/*	URI STATE MACHINE MAP  */

UriStateMap_t	Request::uriStateMap( void )
{
	static	UriStateMap_t state_map;
	static	int i;

	if (i == 0)
	{
		state_map.insert(std::make_pair(std::make_pair(':', SCHEME), AUTH));
		state_map.insert(std::make_pair(std::make_pair('/', SCHEME), PATH));
		state_map.insert(std::make_pair(std::make_pair(':', AUTH), AUTH));
		state_map.insert(std::make_pair(std::make_pair('@', AUTH), HOST));
		state_map.insert(std::make_pair(std::make_pair('/', AUTH), PATH));
		state_map.insert(std::make_pair(std::make_pair('?', AUTH), QUERY));
		state_map.insert(std::make_pair(std::make_pair('#', AUTH), FRAG));
		state_map.insert(std::make_pair(std::make_pair(':', HOST), PORT));
		state_map.insert(std::make_pair(std::make_pair('/', HOST), PATH));
		state_map.insert(std::make_pair(std::make_pair('?', HOST), PATH));
		state_map.insert(std::make_pair(std::make_pair('#', HOST), PATH));
		state_map.insert(std::make_pair(std::make_pair('/', PORT), PATH));
		state_map.insert(std::make_pair(std::make_pair('?', PORT), QUERY));
		state_map.insert(std::make_pair(std::make_pair('#', PORT), FRAG));
		state_map.insert(std::make_pair(std::make_pair('/', PATH), PATH));
		state_map.insert(std::make_pair(std::make_pair('?', PATH), QUERY));
		state_map.insert(std::make_pair(std::make_pair('#', PATH), FRAG));
		state_map.insert(std::make_pair(std::make_pair('#', QUERY), FRAG));
		i++;
	}
	return state_map;
}

/*	URI STATE MACHINE  */

void	Request::parseURIState(states& state, str_t& target, size_t& i)
{
	char c;
	static str_t str;
	static UriStateMap_t state_map = uriStateMap();
	
	while (i < _uri.len) {
		c = _uri.str[i];
		if (isURIdelimited(c, state))
		{
			std::pair<char, states>	state_pair = std::make_pair(c, state);
			UriStateMap_t::iterator	it = state_map.find(state_pair);

			if (it != state_map.end() || i == _uri.len - 1)	{
				if (state == SCHEME)
					setURIPathType(i);
				if (state == AUTH && c == '/')
					_uri.host = str;
				else
					target = str;
				state = it->second;
				str.clear();
				if (c == '/' || c == '?' || c == '#')
					str += c;
				i++;
				return ;
			}
		}
		else
			str += c;
		i++;
	}
	target = str;
	str.clear();
}

void	Request::setURIPathType(size_t& i) {
	
	if (_uri.str.substr(i + 1, 2) == "//") {
		i += 2;
		if (_uri.str[i + 3] == ':')
			_uri.uri_type = ABSOLUTE_FORM;
	}
	else if (_uri.uri_type != ABSOLUTE_FORM && _uri.path.empty() && !_uri.port.empty() && _uri.scheme.empty())
		_uri.uri_type = AUTH_FORM;
}

bool Request::isURIdelimited(char c, enum states state) {
	return (( c == '/' && state != PATH ) || c == '?' || c == '#' || c == ':' || c == '@');
}

/*  URI QUERY PARSE  */

bool	Request::parseQuery(){
/*
	fragment    = *( pchar / "/" / "?" )
	pchar         = unreserved / pct-encoded / sub-delims /

	Locate the ? in the URI to extract the query part.
	Split the query string at & to separate key-value pairs.
	Split each pair at = to extract the key and value.
	Perform URL decoding if necessary (e.g., %20 → space, + → space).
*/	
	if (_uri.query[0] != '?')
		return false;

	std::istringstream iss(_uri.query);
	iss.ignore(1);
	split_stream_to_map(iss, '=', '&');
	return true;
}

bool	Request::split_stream_to_map(std::istringstream& iss, char delim1, char delim2) {
	
	str_t	key;
	str_t	value;
	
	while (iss.eof()) {
		std::getline(iss, key, delim1);
		std::getline(iss, value, delim2);
		_uri.query_map.insert(std::make_pair<str_t, str_t>(key, value));
    }
    return true;
}

/*  HEADER PARSING  */
/* 
	Headers are provided in the ABNF syntax : 
	key: value\r\n
	The header section is delimited by /r/n/r/n
*/
bool	Request::parseHeaders(str_t& str)
{
	std::istringstream	iss(str);
	
	parseStreamToStrVecStrMap(iss, _headers, '\n', ':', ',');
	//parseStrStreamToMap(iss, _headers, '\n', ':');

	std::map<std::string, std::vector< std::string> >::iterator it = _headers.find("mapLastLine");

	if (it->second.size() == 1 && it->second[0].compare("\r") == 0)
	{
		headersKeyToLower();
		_headers.erase("maplastline");
		printStrVecStrMap(_headers);
		return true;
	}
	else
		return false;
}

void	Request::headersKeyToLower( void )
{
	std::map<std::string, std::vector< std::string> > lowered;	
	for (std::map<std::string, std::vector< std::string> >::iterator it = _headers.begin(); it != _headers.end(); it++){
		std::string lowerKey = to_lower(it->first);
		lowered[lowerKey] = it->second;
	}
	_headers = lowered;
	return ;
}

/*  BODY PARSING  */

bool	Request::parseBody(const str_t& str)
{
	std::istringstream stream(str);
	/* INCOMPLETE */
	bool outcome = true;
	return outcome;

	/* 	CONSIDERATIONS 
			
		{
			Transfer-coding Considerations [CHUNKED : need to communicate to server Request State - CHUNKED_INCOMPLETE ] : UNRESOLVED
		} 
		{	Size : [ MAximum Chunk Size? ]										
			: recipients MUST anticipate potentially large hexadecimal			
				numerals and prevent parsing errors due to integer conversion 
					overflows or precision loss due to integer representation } : 
		{
			Last Chunk Validation ; [ CONFIRM NO CHUNK FOLLOWS ZERO CHUNK] : UNRESOLVED - Need to check how end of message is represented CRLF?
		}
		{
		}
	*/
	/*
		Process
		{
			{ Parse size (to CRLF)	: [ VALID HEX CHARS, CONVERT HEX TO DEC ] }	: [CONTINUE ? RETURN INVALID HEX ERROR, CONTINUE]
			{ Validate size			: [ MAXIMUM SIZE CHECK ] }					: [CONTINUE ? RETURN MAX SIZE ERROR, CONTINUE]
			{ Parse chunk (to CRLF)	: [ COMPARE LENGTH TO SIZE ] }				: [CONTINUE ? RETURN LENGTH ERROR]
			{ Validate last chunk	: [ SIZE MUST BE 0 ] }						: [CONTINUE ? RETURN LENGTH ERROR]
		}
 	*/
}




  /* ============= */
 /*  VALIDATION   */
/* ============= */

/*  METHOD VALIDATION  */

Route*	Request::setRoute(){
	return _config->getRouteForTarget(_uri.path);
}

bool	Request::serverSupportsMethod(const str_t& method) {
	const std::vector<str_t> valid_methods = _config->getAllowedMethods();
	std::vector<str_t>::const_iterator it = valid_methods.begin();
	
	for (*it; it != valid_methods.end(); it++) {
		if (method == *it)
			return true;
	}
    return false;
}

bool	Request::validateMethod()
{
	if (serverSupportsMethod(_method)) {
		if (_route && !_route->allowsMethod(_method))
			_error.addErrorFlag(errFlag::METHOD_NOT_PERMITTED);
		return true;
	}
	else if (serverSupportsMethod(to_upper(_method)))
		return _error.addErrorFlag(errFlag::METHOD_CASE);
	else
		return _error.addErrorFlag(errFlag::METHOD_UNRECOGNISED);
}

/*  VERSION VALIDATION  */

bool	Request::validateVersion()
{
	if (_version.compare("HTTP/1.1") == 0 || _version.compare(str_t("HTTP/1.0")) == 0)
		return true ;
	else {
		if (_version.compare(str_t("HTTP/0.9")) == 0 ||
			_version.compare(str_t("HTTP/2.0")) == 0 ||
			_version.compare(str_t("HTTP/3.0")) == 0) {
			return _error.addErrorFlag(errFlag::VERSION_UNSUPPORTED);
		}
		else
			return _error.addErrorFlag(errFlag::VERSION_UNRECOGNISED);
	}
}

  /* ================ */
 /*  URI VALIDATION  */
/* ================ */

bool	Request::validateURI()
{
	bool outcome = true;
	
	if (!validateScheme())	{ outcome = false; }
	if (!validateUser())	{ outcome = false; }
	if (!validatePass())	{ outcome = false; }
	if (!validateHost()) 	{ outcome = false; }
	if (!validatePort())	{ outcome = false; }
	if (!validateQuery())	{ outcome = false; }
	if (!validateFrag())	{ outcome = false; }
	
	return outcome;
}

/*  URI SCHEME VALIDATION  */

bool	Request::validateScheme()
{
	if (_uri.scheme.empty())
		return true;
	to_lower_ref(_uri.scheme);
	if (_uri.scheme.compare("https") == 0 || _uri.scheme.compare("http") == 0)
		return true;
	return _error.addErrorFlag(errFlag::URI_SCHEME_UNSUPPORTED);
}

/*  URI USER VALIDATION  */

bool	Request::validateUser() {
	/* INCOMPLETE */
		/* userinfo = *( unreserved / pct-encoded / sub-delims / ":" ) */
	/* THIS SHOULD PROBABLY RETURN ERROR 400 or 404 
	 	USERNAME / PASS SHOULD NOT APPEAR IN THE  
	*/
	bool outcome = true;

	if (!percentDecode( _uri.user )){ outcome = false; }
	if (!isValidUTF8( _uri.user )){ outcome = false; }
	return outcome;
}

/*  URI PASS VALIDATION  */

bool	Request::validatePass() {
	/* INCOMPLETE */
	bool outcome = true;
	if (!percentDecode( _uri.pass )){ outcome = false;}
	if (!isValidUTF8( _uri.pass )){ outcome = false; }
	return outcome;
}

/*  URI HOST VALIDATION  */

bool	Request::validateHost() {
	bool outcome = true;
	if (!percentDecode( _uri.host ))
		{ outcome = false;}
	if (!isValidUTF8( _uri.host ))
		{ outcome = false; }
	return outcome;
	/* INCOMPLETE */
/* 	
	1) IP literal in [],  

	2) IPv4 address in dotted decimal form
		IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet

      dec-octet   = DIGIT                 ; 0-9
                  / %x31-39 DIGIT         ; 10-99
                  / "1" 2DIGIT            ; 100-199
                  / "2" %x30-34 DIGIT     ; 200-249
                  / "25" %x30-35          ; 250-255

	3) a registered name
		https://www.rfc-editor.org/rfc/rfc1034#section-3.5 or https://www.rfc-editor.org/rfc/rfc1123#section-2
		IP-literal = "[" ( IPv6address / IPvFuture  ) "]"
		IPvFuture  = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
	If a URI containing an IP-literal that starts with "v" return ""address mechanism not supported" - i.e. error 501 Not Implemented
	IPv5
	*/
}

/*  URI PORT VALIDATION  */

bool	Request::validatePort() {
	bool outcome = true;

	if (!(uriCharValidation( CHARSET_PORT, _uri.port )))
		{ outcome = false; }
	if (!_uri.port.empty())
		{_uri.port_int = str_to_int(_uri.port);}
	if (_uri.port_int >= 0 && _uri.port_int <= 65535)
		{ outcome = false; }

	return outcome;
}

/*  URI PATH VALIDATION  */

bool	Request::validatePath() {
/* 1. 
		if ( _uri_path.str.empty() && _

*/
	// if (_uri.uri_type == ABSOLUTE && (!_uri.path.empty() && _uri.path[0] != '//')){
	// 	setError( "Bad Request", "Path must start \'/\' or path must be empty", 400, URI_PATH ); //ERROR MESSAGE NEEDS TO CHANGE
	// 	return false;
	// }
	// else if (_uri.path.empty())
	// 	_uri.path[0] = '//';
	// else if (_uri.uri_type == ABSOLUTE)
	// 	remove_dot_segments(_uri.path);
	// 	else if (_uri.path[0] == '//') {
	// //	merge_path(base_path); // need to get base path from Server Config */
	// 	remove_dot_segments(_uri.path);
	// }
	return true;
}

/*  URI QUERY VALIDATION  */

bool	Request::validateQuery(){
	bool outcome = true;
	if (!percentDecode( _uri.query ))
		{ outcome = false;}
	if (!isValidUTF8( _uri.query ))
		{ outcome = false; }
	return outcome;
}

/*  URI FRAGMENT VALIDATION  */

bool	Request::validateFrag(){
	
	if (_uri.frag.empty())
		return true;
	 return _error.addErrorFlag(errFlag::URI_FRAG_RECEIVED);	
/* 	if (!percentDecode( _uri.frag, URI_FRAG ))
		{ outcome = false;}
	if (!isValidUTF8( _uri.frag ))
		{ outcome = false; } */
}

   /* ===================== */
  /* URI VALIDATION UTILS  */
 /* ===================== */

 bool	Request::percentDecode( str_t& encoded)
 {
	 str_t	decoded;
	 size_t	len = encoded.length();
	 size_t	i = 0;
	 
	 decoded.reserve(len);
	 while ( i < len )
	 {
		 if (encoded[i] == '%')
		 {
			 if ((i + 2) < len && (isxdigit(encoded[i + 1]) && isxdigit(encoded[i + 2]))) {
		 
				 unsigned char converted = hexCharToUnsignedChar(encoded[i + 1]);
				 converted = converted << 4;
				 converted += hexCharToUnsignedChar(encoded[i + 2]);		
				 decoded += converted;
				 i += 3;
			 }
			 else
				return _error.addErrorFlag(errFlag::URI_ENCODING);
			
		 }
		 else
			 decoded += encoded[i++];
	 }
	 encoded = decoded;
	 return true;
 }
 
 bool	Request::uriCharValidation(const str_t set, const str_t& target) {
	 
	 size_t	target_len = target.length();
	 size_t	set_len = set.length();
	 bool	in_set;
	 str_t	message = "URI contains illegal characters";
	 str_t	chars;
 
	 for( size_t i = 0; i < target_len; i++ ) {
		 in_set = false;
		 for( size_t j = 0; j < set_len; j++ ) {
			 if (target[i] == set[j]) {
				 in_set = true ;
				 break;
			 }
		 }
		 if (in_set == false){
			 chars += target[i];
		 }
	 }
	 if (chars.empty())
		 return true;
	 return _error.addErrorFlag(errFlag::URI_ILLEGAL_CHARACTER);
 }

 
 bool	Request::validateBody( void ){
	return true;
 }
 

str_t	Request::enumToURIType() const{
	switch (_uri.uri_type){
		case AUTH_FORM 		:{ return "AUTHORITY FORM"; }
		case ORIGIN_FORM 	:{ return "ORIGIN FORM"; }
		case ASTERISK 		:{ return "ASTERISK FORM"; }
		case ABSOLUTE_FORM	:{ return "ABSOLUTE FORM"; }
		default				:{ return "NOT SET"; 		}
	}
}

/* GETTERS */
str_t	Request::getURIType() 		const	{ return enumToURIType(); }
str_t	Request::getMethodString() 	const	{ return _method; }
str_t	Request::getURIstring() 	const	{ return _uri.str; }
str_t	Request::getURIscheme() 	const	{ return _uri.scheme; }
str_t	Request::getURIuser() 		const	{ return _uri.user; }
str_t	Request::getURIpass() 		const	{ return _uri.pass; }
str_t	Request::getURIhost() 		const	{ return _uri.host; }
str_t	Request::getURIport() 		const	{ return _uri.port; }
int		Request::getURIportInt()			{ return _uri.port_int; }
str_t	Request::getURIpath() 		const	{ return _uri.path; }
str_t	Request::getURIquery() 		const	{ return _uri.query; }
str_t	Request::getURIfrag() 		const	{ return _uri.frag; }
str_t	Request::getVersionString() const	{ return _version; }
int		Request::getResponseCode()			{ return _error.getLastResponseCode(); }

std::ostream& Request::getError(std::ostream& os){
	os << _error << std::endl;
	return os;
}

std::ostream& l14(std::ostream& os)	{ return os << std::setw(14) << std::left; }
std::ostream& r3(std::ostream& os)	{ return os << std::setw(3) << std::right; }

std::ostream&	operator<<(std::ostream& os, Request& request) {

	os << l14 << "URI Type" 		<< r3 << " : " 	<< request.getURIType()			<< "\n";
	os << l14 << "Method String" 	<< r3 << " : " 	<< request.getMethodString()	<< "\n";
	os << l14 << "URI String" 		<< r3 << " : " 	<< request.getURIstring() 		<< "\n";
	os << l14 << "Version String" 	<< r3 << " : "	<< request.getVersionString()	<< "\n\n";
	os << l14 << "Scheme" 			<< r3 << " : " 	<< request.getURIscheme() 		<< "\n";
	os << l14 << "User" 			<< r3 << " : " 	<< request.getURIuser() 		<< "\n";
	os << l14 << "Pass" 			<< r3 << " : "	<< request.getURIpass() 		<< "\n";
	os << l14 << "Host" 			<< r3 << " : " 	<< request.getURIhost() 		<< "\n";
	os << l14 << "Port" 			<< r3 << " : " 	<< request.getURIport() 		<< "\n";
	os << l14 << "Port int" 		<< r3 << " : " 	<< request.getURIportInt()		<< "\n";
	os << l14 << "Path" 			<< r3 << " : " 	<< request.getURIpath() 		<< "\n";
	os << l14 << "Query" 			<< r3 << " : " 	<< request.getURIquery()		<< "\n";
	os << l14 << "Fragment" 		<< r3 << " : " 	<< request.getURIfrag()			<< "\n\n";
	os << l14 << "Last Response"	<< r3 << " : "	<< request.getResponseCode()	<< "\n";
	if (request.getResponseCode() != 200)
		request.getError(os);
	os << std::setw(0) << std::left << std::endl;
	return os;
}

/* TESTING  */

StrVecStrMap_t Request::getRequestHeaders(){
//std::map<std::string, std::vector< std::string> >	Request::getRequestHeaders(){
	return _headers;
}

bool	Request::uriPathHandling(){
	
	
	/* NEED TO DECIDE HOW TO HANDLE PARTIAL PATHS

		1.
			Route : /home/records 
			Request: /records
		
			Do we search within routes for a partial match, and then return a resource?
			Or are we strict, and only allow exact matches?

		2. 
			Route : /home/records/ 
			Request: /home/records/abc/

			Would we return a default file here if abc exists?
			Or are we strict, and only allow exact matches?			

		3.
			Route : /test
			Request: /test/otherFolder/text.html

			Would we return the resource if the path exists, and an error if not? 
			Or are we strict, and only allow exact matches?
			Or do we redirect to /test?
			Or do we alway redirect to /home if the path is invalid?
			
		Options :
			Exact Path Matching	: Only return exact matches
			Dynamic Matching	: Allow partial matches, or treat route path as a base, and allow searches
			Static file routing	: If base matches, and then "parameters" match exactly, return resource
			Default handler		: No match, return default page
			URL rewrite			: If partial match, URL rewrite (example 1 : return test/default_file)
 
		*/
	
	return true;
}

/*
void Request::ChunkedDecoding(){
	
PSEUDO CODE

size_t len = 0;
read chunk-size, chunk-ext (if any), and CRLF
while (chunk-size > 0) {
	read chunk-data and CRLF
	append chunk-data to content
	length := length + chunk-size
	read chunk-size, chunk-ext (if any), and CRLF
}
read trailer field
while (trailer field is not empty) {
	if (trailer fields are stored/forwarded separately) {
		append trailer field to existing trailer fields
	}
	else if (trailer field is understood and defined as mergeable) {
		merge trailer field with existing header fields
	}
	else {
		discard trailer field
	}
	read trailer field
}
Content-Length := length
Remove "chunked" from Transfer-Encoding
}
*/
