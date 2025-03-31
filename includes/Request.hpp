/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:57:35 by kmooney           #+#    #+#             */
/*   Updated: 2025/03/31 01:57:59 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "./core_def.hpp"
#include "../damianServer/Config.hpp"
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
#include <vector>

#include <unistd.h>

enum path_types{ PARTIAL, ABSOLUTE };
enum states { SCHEME, AUTH, USERINFO, HOST, PORT, PATH, QUERY, FRAG };
enum uri_types { HTTP, HTTPS, DOUBLE_ENCODING, HTTP_REDIRECT, IMPROPER_ENCODING, INVALID_HOST, INVALID_PORT, INVALID_SCHEME, UNSUPPORTED_SCHEME};

typedef std::map<str_t, str_t> StringMap_t;
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

// generic URI delimiters  = :/?#[]@
// sub-delims  = !$&'()*+,;=
//# define RESERVED		"!#$&'()*+,/:;=?@[]"
//# define UNRESERVED	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~"
# define SCHEME_CHARS	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-."
# define AUTH_CHARS		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789%!$&'()*+,;="
# define HOST_CHARS		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789%!$&'()*+,;=."
# define PORT_CHARS		"0123456789"
class	Request
{
	public :
		struct uri {	
			StringMap_t						query_map;
			enum uri_types					uri_type;
			enum path_types					path_type;
			size_t							len;
			str_t 					frag, host, pass, path, port, query, scheme, str, target, user;
			int								port_int;

			uri(): uri_type(), path_type(PARTIAL), len(0), frag(""), host(""), pass(""), path(""), port(""), query(""), 
			scheme(""), str(""), user(""), port_int(80){}
		};

				/* CONSTRUCTORS */
				Request( void );
				Request( const str_t& str );
				Request( const Request& other );
				Request( const str_t& str, Config* config );
			
				/* DESTRUCTORS */
				~Request( void );
			
				/* COPY ASSIGNMENT */
				Request&		operator=(const Request& other);
			
				/*	PARSING	*/
				bool			parseRequestLine(const str_t& str);
				bool			parseHeaders(const str_t& str);
				bool			parseBody(const str_t& str);
			
				/* SETTERS */
				void			setError(const str_t& str1, const str_t& str2, int num);
			
				/*	FETCH	*/
				Route*			setRoute();
				bool			serverSupportsMethod(const str_t& method);
				//bool			serverSupportsMethod();
				bool			routeSupportsMethod();
				
				/* GETTERS */
				str_t			enumToVersionType() const;
				str_t			getMethodType() const;
				str_t			getMethodString() const;
				str_t			getURIfrag() const;
				str_t			getURIhost() const;
				str_t			getURIpath() const;
				str_t			getURIport() const;
				int				getURIportInt();
				str_t			getURIquery() const;
				str_t			getURIscheme() const;
				str_t			getURIstring() const;
				str_t			getURIuser() const;
				str_t			getURIpass() const;
				str_t			getVersionString() const;
				str_t			getVersionType() const;
				int				getResponseCode();
				std::ostream&	getError(std::ostream& os);
				void			printErrors(std::ostream& os) const;
			
				/* TESTING */			
				StringMap_t		getRequestHeaders();
				
				
				str_t	_request_line, _header_line, _body, _method, _version;
				uri			_uri;
				
				StringMap_t	_headers;
				int			_last_response_code;
				Error		_error;
				Config*		_config;
				Route*		_route;
				
				str_t	_target_path;
				str_t	_target_file;
				
				/*	PARSING	*/
				bool			parseRequestLine();
				bool			parseMethod(std::istringstream& stream);
				bool			parseVersion(std::istringstream& stream);
				
				/*	URI PARSING	*/
				bool			parseURI(std::istringstream& stream);
				void			parseURIState(states& state, str_t& target, size_t& i);
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
				
	private:
		/*	URI FURTHER HANDLING - POSSIBLY FOR REQUEST HANDLER	*/
		bool			uriPathHandling();
		bool			uriPathRemoveDots();
		bool			uriPathRouteFinder();


		/*	URI VALIDATION UTILS */
		bool			percentDecode(str_t& encoded);
		bool			uriCharValidation(const str_t set, const str_t& target);
		
};

/* OUTPUT FORMAT  */
std::ostream& l14(std::ostream& os);
std::ostream& r3(std::ostream& os);

/* PRINTING REQUEST */
std::ostream&	operator<<(std::ostream& os, Request& request);
#endif
