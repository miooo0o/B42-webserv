/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:57:35 by kmooney           #+#    #+#             */
/*   Updated: 2025/03/17 07:21:34 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "./TestClasses/testUtils.hpp"
#include "./utility_functions.hpp"

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

class	Request
{
	// generic URI delimiters  = :/?#[]@
	// sub-delims  = !$&'()*+,;=
	//# define RESERVED		"!#$&'()*+,/:;=?@[]"
	//# define UNRESERVED	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~"
	# define SCHEME_CHARS	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-."
	# define AUTH_CHARS		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789%!$&'()*+,;="
	# define HOST_CHARS		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789%!$&'()*+,;=."
	# define PORT_CHARS		"0123456789"

	enum err_loc { METHOD, URI_SCHEME, URI_USER, URI_PASS, URI_HOST, URI_PORT, URI_PATH, URI_QUERY, URI_FRAG, VERSION, HEADERS, BODY };
	enum method_types { GET, POST, DELETE, UNSUPPORTED_METHOD, BAD_CASE, UNRECOGNISED_METHOD };
	enum path_types{ ABSOLUTE, PARTIAL };
	enum states { SCHEME, AUTH, USERINFO, HOST, PORT, PATH, QUERY, FRAG };
	enum uri_types { HTTP, HTTPS, DOUBLE_ENCODING, HTTP_REDIRECT, IMPROPER_ENCODING, INVALID_HOST, INVALID_PORT, INVALID_SCHEME, UNSUPPORTED_SCHEME};
	enum version_types { OPO, OPZ, BAD_REQUEST, UNSUPPORTED_VERSION, UNRECOGNISED_VERSION };
	
	struct request_error{
		int												num;
		enum err_loc									err_location;
		std::string										str1;
		std::string										str2;
	};
	
	struct method {
		enum method_types								type;
		std::string										str;
		method():type(), str(""){}
	};
	
	struct uri {
		std::map<std::string, std::string>				fragments;				
		std::map<std::string, std::string>				query;				
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
		int												port_int;
		std::string 									user;
		
		uri(): uri_type(), path_type(), len(0), frag(""), host(""), path(""), port(""), query(""), 
			scheme(""), str(""), user(""), pass(""), port_int(80){}
	};
	
	struct version{
		enum version_types								type;
		std::string										str;
		int												major;
		int												minor;
		version(): type(), str(""), major(0), minor(0){}
	};
	
	private:
			std::string									_header_line;
			std::string									_request_line;
			std::string									_body;
			
			method										_method;
			uri											_uri;
			version										_version;
			
			std::map<std::string, std::string>			_headers;

			std::list<request_error>					_errors;
		
			int											_last_response_code;
			enum err_loc								_last_error_loc;

							/* URI STATE MAP */
							
							/*	PARSING	*/
			bool			parseRequestLine();
			bool			parseMethod(std::istringstream& stream);
			bool			parseVersion(std::istringstream& stream);
			bool			parseURI(std::istringstream& stream);
			void			parseURIState(states& state, std::string& target, size_t& i);
			std::map<std::pair<char, states>, states> uriStateMap( void );
			bool			parseFragment();
			bool			parseQuery();
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
			bool			percentDecode(std::string& encoded, err_loc err_location);
			bool			uriCharValidation(const std::string set, const std::string& target, err_loc err_location);
			
	public:
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
			void			setError(const std::string& str1, const std::string& str2, int num, err_loc err_location);
			void			setMethod(method_types type, std::string& str);
			void			setVersion(version_types type, std::string& str, int maj, int min);
							
							/* GETTERS */
			std::string		getErrorLocation(enum err_loc err_location) const;
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

			void			printErrors(std::ostream& os) const;
};

/* OUTPUT FORMAT  */
std::ostream& l14(std::ostream& os);
std::ostream& r3(std::ostream& os);

/* PRINTING REQUEST */
std::ostream&	operator<<(std::ostream& os, Request& request);
#endif
