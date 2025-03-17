/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:04:15 by kmooney           #+#    #+#             */
/*   Updated: 2025/03/17 07:21:34 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"

/*	DEFAULT CONSTRUCTOR
	
	PARSE REQUEST LINE MUST THEN BE CALLED WITH REQUEST LINE
	CAN USE THE RETURN VALUE PARSE REQUEST LINE (BOOL) TO DETERMINE IF ERROR OR NOT
	- I PREFER NOT TO USE THIS APPROACH
*/
Request::Request() : _body(""), _header_line(""), _request_line(""), 
	_errors(), _headers(), _method(), _uri(), _version(), _last_response_code(200) {
}

/*	PARAMETRISED CONSTRUCTOR - TAKES REQUEST LINE AS PARAMETER
	
	PARSE REQUEST MUST THEN CALLED WITHOUT REQUEST LINE (OR COULD CALL PARSE)
	CAN CHECK REQUEST OBJECT FOR ERROR
*/
Request::Request( const std::string& str ) : _body(""), _header_line(""), _request_line(str), 
	_errors(), _headers(), _method(), _uri(), _version(), _last_response_code(200) {
	parseRequestLine();
}

Request::~Request( void ) {}

/* NOT SURE THIS IS NECESSARY - WE COULD MAKE PRIVATE */
Request::Request( const Request& other ) : _body(other._body),  _header_line(other._header_line),
	_request_line(other._request_line), _errors(other._errors), _headers(other._headers),
	 _method(other._method), _uri(other._uri), _version(other._version), _last_response_code(other._last_response_code) {
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
bool	Request::parseRequestLine(const std::string& str)
{
	std::istringstream stream(str);

	bool outcome = true;
	if (!(parseMethod(stream) && validateMethod()))
		outcome = false;
	if (!(parseURI(stream) && validateURI()))
		outcome = false;
	if (!(parseVersion(stream) && validateVersion()))
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
	std::istringstream stream(_request_line);

	bool outcome = true;
	if (!(parseMethod(stream) && validateMethod()))
		outcome = false;
	if (!(parseURI(stream) && validateURI()))
		outcome = false;
	if (!(parseVersion(stream) && validateVersion()))
		outcome = false;
	return outcome;
}

/*  METHOD PARSING  */

bool	Request::parseMethod(std::istringstream& stream)
{
	std::getline(stream, _method.str, ' ');
	if (_method.str.empty())
		return false;
	return true;
}

/*  VERSION PARSING  */

bool	Request::parseVersion(std::istringstream& stream)
{
	std::getline(stream, _version.str, '\r');

	char ch;
    if (stream.get(ch) && ch == '\n') {
		if (_version.str.empty()) {
			setError( "Bad Request", "No HTTP version provided", 400, VERSION );
			setVersion (BAD_REQUEST, _version.str, 0, 0);
			return false;
		}
		return true;
	}
	else {
		setError( "Bad Request", "Request Line must end \\r\\n", 400, VERSION );
		setVersion (BAD_REQUEST, _version.str, 0, 0);
	}
	return false;
}

   /* ============= */
  /*  URI PARSING  */
 /* ============= */

bool	Request::parseURI(std::istringstream& stream)
{
	std::string	str;
	states		state = SCHEME;
	size_t		i = 0;

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

std::map<std::pair<char, Request::states>, Request::states>	Request::uriStateMap( void )
{
	static	std::map<std::pair<char, states>, states> state_map;
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

void	Request::parseURIState(states& state, std::string& target, size_t& i)
{
	char c;
	static std::string str;
	static std::map<std::pair<char, Request::states>, Request::states> state_map = uriStateMap();
	
	while (i < _uri.len){
		c = _uri.str[i];
		if (( c == '/' && state != PATH ) || c == '?' || c == '#' || c == ':' || c == '@')
		{
			std::pair< char, Request::states > test = std::make_pair(c, state);
			std::map< std::pair <char, Request::states>, Request::states>::iterator it = state_map.find(test);

			if (it != state_map.end() || i == _uri.len - 1)
			{
				if (state == SCHEME && _uri.str.substr(i + 1, 2) == "//")
					i += 2;
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

/*  URI FRAGMENT PARSE  */

bool	Request::parseFragment(){}

/*  URI QUERY PARSE  */

bool	Request::parseQuery(){}

/*  HEADER PARSING  */

bool	Request::parseHeaders(const std::string& str)
{
	std::istringstream stream(str);
	/* INCOMPLETE */ 
	bool outcome = true;
	return outcome;
}

/*  BODY PARSING  */

bool	Request::parseBody(const std::string& str)
{
	std::istringstream stream(str);
	/* INCOMPLETE */
	bool outcome = true;
	return outcome;
}

  /* ============= */
 /*  VALIDATION   */
/* ============= */

/*  METHOD VALIDATION  */

bool	Request::validateMethod()
{
	const std::set<std::string> valid_methods = get_valid_methods();
	const std::set<std::string> unsupported_methods = get_unsupported_methods();

	if (valid_methods.find(_method.str) != valid_methods.end())
	{
		if (_method.str == "GET") _method.type = GET;
		if (_method.str == "POST") _method.type = POST;
		if (_method.str == "DELETE") _method.type = DELETE;
		return true;
	}
	else if (valid_methods.find(to_upper(_method.str)) != valid_methods.end()){
		_method.type = BAD_CASE;
		setError( "Bad Request", "Invalid HTTP method: must be uppercase", 400, METHOD);
	}
	else if (unsupported_methods.find(_method.str) != valid_methods.end()){
		_method.type = UNSUPPORTED_METHOD;
		setError( "Bad Request", "Method not supported", 400, METHOD);
	}
	else {
		_method.type = UNRECOGNISED_METHOD;
		setError( "Bad Request", "Method not recognised", 400, METHOD);
	}
	return false;
}

/*  VERSION VALIDATION  */

bool	Request::validateVersion()
{
	if (_version.str.compare("HTTP/1.1") == 0)
		setVersion(OPO, _version.str, 1, 1);
	else if (_version.str.compare(std::string("HTTP/1.0")) == 0)
		setVersion(OPZ, _version.str, 1, 0);
	else if (_version.str.compare(std::string("HTTP/0.9")) == 0 ||
				_version.str.compare(std::string("HTTP/2.0")) == 0 ||
					_version.str.compare(std::string("HTTP/3.0")) == 0) {
		setError("Bad Request", "Unsupported HTTP Version : " + _version.str, 400, VERSION);
		setVersion(UNSUPPORTED_VERSION, _version.str, substring_to_int(_version.str, 5, 1), 0);
	}
	else {
		setVersion(UNRECOGNISED_VERSION, _version.str, 0, 0);
		setError("Bad Request", "Unrecognised HTTP Version : " + _version.str, 400, VERSION);
	}
	if (_version.type > 1)
		return false;
	return true ;
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
	/* INCOMPLETE */
	//scheme      = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
	const std::set<std::string> unsupported_schemes = get_unsupported_schemes();

	to_lower_ref(_uri.scheme);
	if (_uri.scheme.compare("https") == 0 || _uri.scheme.compare("http") == 0)
		return true;
	uriCharValidation(SCHEME_CHARS, _uri.scheme, URI_SCHEME);
	if (unsupported_schemes.find(to_lower(_uri.scheme)) != unsupported_schemes.end()){
		_uri.uri_type = UNSUPPORTED_SCHEME;
		setError( "Bad Request", "Scheme not supported : " + _uri.scheme, 400, URI_SCHEME);
	}
	else {
		_uri.uri_type = INVALID_SCHEME;
		setError( "Bad Request", "URI Scheme not recognised", 400, URI_SCHEME);
	}
	return false;
}

/*  URI USER VALIDATION  */

bool	Request::validateUser() {
	/* INCOMPLETE */
		/* userinfo = *( unreserved / pct-encoded / sub-delims / ":" ) */
	/* THIS SHOULD PROBABLY RETURN ERROR 400 or 404 
	 	USERNAME / PASS SHOULD NOT APPEAR IN THE  
	*/
	bool outcome = true;

	if (!percentDecode( _uri.user, URI_USER )){ outcome = false; }
	if (!isValidUTF8( _uri.user )){ outcome = false; }
	return outcome;
}

/*  URI PASS VALIDATION  */

bool	Request::validatePass() {
	/* INCOMPLETE */
	bool outcome = true;
	if (!percentDecode( _uri.pass, URI_PASS )){ outcome = false;}
	if (!isValidUTF8( _uri.pass )){ outcome = false; }
	return outcome;
}

/*  URI HOST VALIDATION  */

bool	Request::validateHost() {
	bool outcome = true;
	if (!percentDecode( _uri.host, URI_HOST ))
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

	if (!(uriCharValidation( PORT_CHARS, _uri.port, URI_PORT)))
		{ outcome = false; }
	if (!_uri.port.empty())
		{_uri.port_int = str_to_int(_uri.port);}
	else if (_uri.uri_type == HTTPS)
		{_uri.port_int = 443;}
	if (_uri.port_int >= 0 && _uri.port_int <= 65535)
		{ outcome = false; }
	
	return outcome;
}

/*  URI PATH VALIDATION  */

bool	Request::validatePath() {
 /* if authority i.e.[userinfo/host/port] either
 	1)  path starts '/'  
	2)  must be empty */

	// NEED TO REMOVE PATH TRAVERSAL ELEMENTS
	return true;
}

/*  URI QUERY VALIDATION  */

bool	Request::validateQuery(){
	bool outcome = true;
	if (!percentDecode( _uri.query, URI_QUERY ))
		{ outcome = false;}
	if (!isValidUTF8( _uri.query ))
		{ outcome = false; }
	return outcome;
}

/*  URI FRAGMENT VALIDATION  */

bool	Request::validateFrag(){
	bool outcome = true;
	if (!percentDecode( _uri.frag, URI_FRAG ))
		{ outcome = false;}
	if (!isValidUTF8( _uri.frag ))
		{ outcome = false; }
	return outcome;
}

   /* ===================== */
  /* URI VALIDATION UTILS  */
 /* ===================== */

 bool	Request::percentDecode( std::string& encoded, err_loc err_location )
 {
	 std::string	decoded;
	 size_t		len = encoded.length();
	 size_t		i = 0;
	 
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
			 {
				 setError("Bad Request", "Invalid Percent Encoding", 400, err_location);
				 return false;
			 }
		 }
		 else
			 decoded += encoded[i++];
	 }
	 encoded = decoded;
	 return true;
 }
 
 bool	Request::uriCharValidation(const std::string set, const std::string& target, err_loc err_location) {
	 
	 size_t		target_len = target.length();
	 size_t		set_len = set.length();
	 bool		in_set;
	 std::string	message = "URI contains illegal characters";
	 std::string chars;
 
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
	 setError( "Bad Request", message + " \"" + chars + "\"", 400, err_location );
	 return false;
 }

/* SETTERS */
void	Request::setError(const std::string& str1, const std::string& str2, int num, err_loc err_location) {
	request_error error;
	error.num = num;
	error.err_location = err_location;
	error.str1 = str1;	
	error.str2 = str2;	
	_last_response_code = num;
	_last_error_loc = err_location;
	_errors.push_back(error);
}

void	Request::setMethod(method_types type, std::string& str) {
	_method.type = type;
	_method.str = str;
}

void	Request::setVersion(version_types type, std::string& str, int maj, int min) {
	_version.type = type;
	_version.str = str;
	_version.major = maj;
	_version.minor = min;
}

/* GETTERS */

std::string	Request::getMethodType() const{
	
	std::string type;

	switch( _method.type ){
		case GET 					: { type = "GET"; break; }
		case POST 					: { type = "POST"; break; }
		case DELETE 				: {	type = "DELETE"; break;	}
		case BAD_CASE 				: { type = "BAD CASE"; break;	}
		case UNSUPPORTED_METHOD 	: {	type = "UNSUPPORTED"; break; }
		case UNRECOGNISED_METHOD 	: { type = "UNRECOGNISED";	break; }
	};
	return type;
}

std::string	Request::getVersionType() const{
	
	std::string type;

	switch( _version.type ){
		case OPO 					: { type = "HTTP/1.1"; break; }
		case OPZ 					: { type = "HTTP/1.0"; break; }
		case BAD_REQUEST 			: { type = "BAD_REQUEST"; break; }
		case UNSUPPORTED_VERSION 	: { type = "UNSUPPORTED VERSION"; break; }
		case UNRECOGNISED_VERSION 	: { type = "UNRECOGNISED VERSION"; break; }
	};
	return type;
}

std::string	Request::getErrorLocation(enum err_loc err_location) const{
	switch(err_location){
		case METHOD		: { return("REQUEST LINE : METHOD"); break; }
		case URI_SCHEME	: { return("REQUEST LINE : URI SCHEME"); break; }
		case URI_USER	: { return("REQUEST LINE : URI USER"); break; }
		case URI_PASS	: { return("REQUEST LINE : URI PASS"); break; }
		case URI_HOST	: { return("REQUEST LINE : URI HOST"); break; }
		case URI_PORT	: { return("REQUEST LINE : URI PORT"); break; }
		case URI_PATH	: { return("REQUEST LINE : URI PATH"); break; }
		case URI_QUERY	: { return("REQUEST LINE : URI QUERY"); break; }
		case URI_FRAG	: { return("REQUEST LINE : URI FRAGMENT"); break; }
		case VERSION	: { return("REQUEST LINE : VERSION"); break; }
		case HEADERS	: { return("HEADERS "); break; }
		case BODY		: { return("BODY"); break; }
		default			:  break;
	}
	std::string str = "NONE";
	return str;
 }

std::string	Request::getMethodString() const	{ return _method.str; }
std::string	Request::getURIstring() const		{ return _uri.str; }
std::string	Request::getURIscheme() const		{ return _uri.scheme; }
std::string	Request::getURIuser() const			{ return _uri.user; }
std::string	Request::getURIpass() const			{ return _uri.pass; }
std::string	Request::getURIhost() const			{ return _uri.host; }
std::string	Request::getURIport() const			{ return _uri.port; }
int			Request::getURIportInt()			{ return _uri.port_int; }
std::string	Request::getURIpath() const			{ return _uri.path; }
std::string	Request::getURIquery() const		{ return _uri.query; }
std::string	Request::getURIfrag() const			{ return _uri.frag; }
std::string	Request::getVersionString() const	{ return _version.str; }
int			Request::getResponseCode()			{ return _last_response_code; }
std::string	Request::getLastErrorLoc()			{ return getErrorLocation(_last_error_loc); }

void	Request::printErrors(std::ostream& os) const {

	int	i = 0;
	
	if (_errors.begin() == _errors.end())
		return ;
	os << "Request Errors:\n";
	for (std::list<request_error>::const_iterator it = _errors.begin(); it != _errors.end(); ++it)
	{
		os << "\nError " << i << "\n";
		os << "=======" << "\n";
		
		os << l14 << "Error num" << r3 << " : " << it->num << "\n";
		os << l14 << "Error str" << r3 << " : " << it->str1 << "\n";
		os << l14 << "Error str" << r3 << " : " << it->str2 << "\n";
		os << l14 << "Error loc" << r3 << " : " << getErrorLocation(it->err_location) << "\n";
		
		i++;
	}
}

std::ostream& l14(std::ostream& os)	{ return os << std::setw(14) << std::left; }
std::ostream& r3(std::ostream& os)	{ return os << std::setw(3) << std::right; }


std::ostream&	operator<<(std::ostream& os, Request& request) {

	os << l14 << "Method String" 	<< r3 << " : " 	<< request.getMethodString()	<< "\n";
	os << l14 << "Method" 			<< r3 << " : "	<< request.getMethodType()		<< "\n\n"; 
	os << l14 << "URI String" 		<< r3 << " : " 	<< request.getURIstring() 		<< "\n";
	os << l14 << "Scheme" 			<< r3 << " : " 	<< request.getURIscheme() 		<< "\n";
	os << l14 << "User" 			<< r3 << " : " 	<< request.getURIuser() 		<< "\n";
	os << l14 << "Pass" 			<< r3 << " : "	<< request.getURIpass() 		<< "\n";
	os << l14 << "Host" 			<< r3 << " : " 	<< request.getURIhost() 		<< "\n";
	os << l14 << "Port" 			<< r3 << " : " 	<< request.getURIport() 		<< "\n";
	os << l14 << "Port int" 		<< r3 << " : " 	<< request.getURIportInt()		<< "\n";
	os << l14 << "Path" 			<< r3 << " : " 	<< request.getURIpath() 		<< "\n";
	os << l14 << "Query" 			<< r3 << " : " 	<< request.getURIquery()		<< "\n";
	os << l14 << "Fragment" 		<< r3 << " : " 	<< request.getURIfrag()			<< "\n\n";
	os << l14 << "Version String" 	<< r3 << " : "	<< request.getVersionString()	<< "\n";
	os << l14 << "Last Response"	<< r3 << " : "	<< request.getResponseCode()	<< "\n\n";
	request.printErrors(os);
	os << std::setw(0) << std::left << std::endl;
	return os;
}
