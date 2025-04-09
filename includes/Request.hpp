
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:57:35 by kmooney           #+#    #+#             */
/*   Updated: 2025/04/07 13:44:34 by kmooney          ###   ########.fr       */
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

enum uri_types{ AUTH_FORM, ORIGIN_FORM, ABSOLUTE_FORM, ASTERISK };
enum states { SCHEME, AUTH, USERINFO, HOST, PORT, PATH, QUERY, FRAG };
typedef	std::map<std::pair<char, states>, states> UriStateMap_t;

// generic URI delimiters  = :/?#[]@
// sub-delims  = !$&'()*+,;=
//# define RESERVED		"!#$&'()*+,/:;=?@[]"
//# define UNRESERVED	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~"
# define CHARSET_SCHEME	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-."
# define CHARSET_AUTH	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789%!$&'()*+,;="
# define CHARSET_HOST	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789%!$&'()*+,;=."
# define CHARSET_PORT	"0123456789"
# define CHARSET_HEX	"0123456789ABCDEF"
# define CHARSET_DEC	"0123456789"
# define CRLF			"\r\n"

class	Request
{
	public :
		struct uri {
			enum uri_types			uri_type;
			size_t					len;
			int						port_int;
			str_t 					frag, host, pass, path, port, query, scheme, str, target, user;
			std::map<std::string, std::string> query_map;

			uri(): uri_type(ORIGIN_FORM), len(0), port_int(80), frag(""), host(""), pass(""), path(""), port(""), query(""),
			scheme(""), str(""), user(""){}
		};

			/* CONSTRUCTORS */
			Request( void );
			Request( const str_t& str );
			Request( const str_t& str, Config* config );

			/* DESTRUCTORS */
			~Request( void );


			/*	PARSING	*/
			bool			parseRequestLine(const str_t& str);
			bool			parseHeaders(str_t& str);
			bool			parseBody(const str_t& str);

			/* SETTERS */
			void			setError(const str_t& str1, const str_t& str2, int num);

			/*	FETCH	*/
			Route*			setRoute();
			bool			serverSupportsMethod(const str_t& method);
			bool			routeSupportsMethod();

			/* GETTERS */

			Config*			getConfig( void ) const { return (_config); } // FIXME(@mina): !!! I need to access Config -> @kev
			str_t			enumToURIType() const;
			str_t			getURIType() const;
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
			std::map<std::string, std::vector< std::string> > getRequestHeaders();
						/*	TEMPORARY TEST FUNCTIONS	*/
			bool			test_validateHeaders( void );

			private:
			/* EXCEPTIONS */
			class	HeaderException : public std::exception
			{
				public:
					uint64_t _errnum;

					HeaderException( uint64_t errnum ) : _errnum(errnum){throw error();};
					virtual ~HeaderException() throw(){};
					virtual uint64_t error() throw();

					uint64_t getValue() const { return _errnum; }
			};

			str_t			_request_line, _header_line, _body, _method, _version;
			uri				_uri;

			std::map<std::string, std::vector< std::string> > _headers;
			int				_last_response_code;
			Error			_error;
			Config*			_config;
			Route*			_route;

			str_t			_target_path;
			str_t			_target_file;

			/* COPY CONSTRUCTOR */
			Request( const Request& other );
			/* COPY ASSIGNMENT */
			Request&		operator=(const Request& other);

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

			/*	HEADER VALIDATION	*/
			uint64_t		error() const;
			void			headerCheck( const str_t header, void (*f)(str_t));

			static void		headersTransferEncoding( str_t val );
			static void		headersHost( str_t val );
			void			headersHostChecks( void ) const;
			void			headersKeyToLower( void );

			/*	URI VALIDATION	*/
			bool			validateScheme( void );
			bool			validateUser( void );
			bool			validatePass( void );
			bool			validateHost( void );
			bool			validatePort( void );
			bool			validatePath( void );
			bool			validateQuery( void );
			bool			validateFrag( void );

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
