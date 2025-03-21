/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:57:35 by kmooney           #+#    #+#             */
/*   Updated: 2025/03/21 14:58:38 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "./TestClasses/testUtils.hpp"
#include "./utility_functions.hpp"
#include "./Error.hpp"

#include <cstring>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <stdint.h>
#include <unistd.h>
#include <vector>

enum method_types { GET, POST, DELETE};
enum path_types{ PARTIAL, ABSOLUTE };
enum states { SCHEME, AUTH, USERINFO, HOST, PORT, PATH, QUERY, FRAG };
enum uri_types { HTTP, HTTPS, DOUBLE_ENCODING, HTTP_REDIRECT, IMPROPER_ENCODING, INVALID_HOST, INVALID_PORT, INVALID_SCHEME, UNSUPPORTED_SCHEME};
enum version_types { OPO, OPZ};

typedef std::map<std::string, std::string> StringMap_t;
typedef	std::map<std::pair<char, states>, states> UriStateMap_t;


/*
#define CODE400			"Bad Request"
#define HEAD_END		"400 Headers: Incorrect character sequence at end of headers"
#define HEAD_SEP		"400 Headers: entries must be separated by /\r/\n"
#define METH_NOT_PER	"405 Method : Method not permitted for the target resource"
#define METH_UNREC		"501 Method : Method not recognised"
#define METH_UNSUP		"501 Method : Method not supported"
#define METH_CASE		"400 Method : Invalid Format: must be uppercase"
#define REQ_END			"400 Request Line : Incorrect character sequence at end of request line"
#define URI_ENCOD		"400 URI : Invalid Percent Encoding"
#define URI_FRAG_REC	"400 URI : Fragment : Element Forbidden"
#define URI_PATH_INV	"400 URI : Path is invalid : "
#define URI_SCH_UNSUP	"400 URI : Scheme not supported : "
#define URI_SCH_UNREC	"400 URI : Scheme not recognised : "
#define VER_NONE		"400 Version : HTTP version not provided"
#define VER_UNREC		"400 Version : Unrecognised HTTP Version : "
#define VER_UNSUP		"400 Version : Unsupported HTTP Version : " */
class	Request
{
	public :
	// generic URI delimiters  = :/?#[]@
	// sub-delims  = !$&'()*+,;=
	//# define RESERVED		"!#$&'()*+,/:;=?@[]"
	//# define UNRESERVED	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~"
	# define SCHEME_CHARS	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-."
	# define AUTH_CHARS		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789%!$&'()*+,;="
	# define HOST_CHARS		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789%!$&'()*+,;=."
	# define PORT_CHARS		"0123456789"
	
	struct request_error{
		int												num;
		std::string										str1;
		std::string										str2;
	};
	
	struct method {
		enum method_types								type;
		std::string										str;
		method():type(), str(""){}
	};
	
	struct uri {	
		StringMap_t										query_map;
		enum uri_types									uri_type;
		enum path_types									path_type;
		size_t											len;
		std::string 									frag;
		std::string 									host;
		std::string 									pass;
		std::string 									path;
		std::string										port;
		std::string 									query;
		std::string 									scheme;
		std::string 									str;
		std::string 									target;
		std::string 									user;
		int												port_int;
		
		uri(): uri_type(), path_type(PARTIAL), len(0), frag(""), host(""), pass(""), path(""), port(""), query(""), 
		scheme(""), str(""), user(""), port_int(80){}
	};
	
	struct version{
		enum version_types								type;
		std::string										str;
		version(): type(), str(""){}
	};
	
	private:
	std::string									_request_line;
	std::string									_header_line;
	std::string									_body;
	
	method										_method;
	uri											_uri;
	version										_version;
	
	StringMap_t									_headers;
	
	std::list<request_error>					_errors;
	
	int											_last_response_code;
	
	/*	PARSING	*/
	bool			parseRequestLine();
	bool			parseMethod(std::istringstream& stream);
	bool			parseVersion(std::istringstream& stream);
	
	/*	URI PARSING	*/
	bool			parseURI(std::istringstream& stream);
	void			parseURIState(states& state, std::string& target, size_t& i);
	UriStateMap_t	uriStateMap( void );
	void 			setURIPathType(size_t& i);
	bool			isURIdelimited(char c, states state);
	
	bool			parseFragment();
	bool			parseQuery();
	bool			split_stream_to_map(std::istringstream& iss, char delim1, char delim2);
	bool			parseHeaders();
	bool			parseBody();
	
	/*	VALIDATION	*/
	bool			validateBody( void );
	bool			validateHeaders( void );
	bool			validateMethod( void );
	bool			validateURI( void );
	bool			validateVersion( void );
	
	/*	URI VALIDATION	*/
	bool			validateScheme( void );
	bool			validateUser( void );
	bool			validatePass( void );
	bool			validateHost( void );
	bool			validatePort( void );
	bool			validatePath( void );
	bool			validateQuery( void );
	bool			validateFrag( void );
	
	/*	URI VALIDATION UTILS */
	bool			percentDecode(std::string& encoded);
	bool			uriCharValidation(const std::string set, const std::string& target);
	
	public:
		Error		_testError;
	
	/* CONSTRUCTORS */
	Request( void );
	Request( const std::string& str );
	Request( const Request& other );
	
	/* DESTRUCTORS */
	~Request( void );
	
	/* COPY ASSIGNMENT */
	Request&		operator=(const Request& other);
	
	/*	PARSING	*/
	bool			parseRequestLine(const std::string& str);
	bool			parseHeaders(const std::string& str);
	bool			parseBody(const std::string& str);
	
	/* SETTERS */
	void			setError(const std::string& str1, const std::string& str2, int num);
	void			setMethod(method_types type, std::string& str);
	void			setVersion(version_types type, std::string& str);
	
	/* GETTERS */
	std::string		enumToVersionType() const;
	std::string		getMethodType() const;
	std::string		getMethodString() const;
	std::string		getURIfrag() const;
	std::string		getURIhost() const;
	std::string		getURIpath() const;
	std::string		getURIport() const;
	int				getURIportInt();
	std::string		getURIquery() const;
	std::string		getURIscheme() const;
	std::string		getURIstring() const;
	std::string		getURIuser() const;
	std::string		getURIpass() const;
	std::string		getVersionString() const;
	std::string		getVersionType() const;
	int				getResponseCode();
	std::string		getLastErrorLoc();
	std::ostream&	getError(std::ostream& os);
	void			printErrors(std::ostream& os) const;
	
	/* TESTING */			
	StringMap_t		getRequestHeaders();
};

/* OUTPUT FORMAT  */
std::ostream& l14(std::ostream& os);
std::ostream& r3(std::ostream& os);

/* PRINTING REQUEST */
std::ostream&	operator<<(std::ostream& os, Request& request);
#endif
