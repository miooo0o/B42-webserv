/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 10:45:02 by kmooney           #+#    #+#             */
/*   Updated: 2025/04/02 20:37:43 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	Header				Description														Example
	======				============													========
	Host				Specifies the target hostname (required in HTTP/1.1).			Host: example.com
	User-Agent			Identifies the client software making the request.				User-Agent: Mozilla/5.0
	Accept				Informs the server about content types the client can process.	Accept: text/html, application/json
	Accept-Encoding		Specifies encoding the client supports (e.g., compression).		Accept-Encoding: gzip, deflate
	Accept-Language		Preferred language for the response.							Accept-Language: en-US,en;q=0.5
	Connection			Determines if the connection should be kept open.				Connection: keep-alive
	Content-Type		Indicates the media type of the request body (e.g., for POST).	Content-Type: application/x-www-form-urlencoded
	Content-Length		Specifies the size of the request body in bytes.				Content-Length: 34
	Authorization		Used for authentication with the server.						Authorization: Basic dXNlcm5hbWU6cGFzc3dvcmQ=
	Cookie				Sends stored cookies to the server.								Cookie: session_id=abc123
	Referer				Specifies the URL of the referring page.						Referer: https://google.com
	If-Modified-Since	Requests a resource only if it has changed since a given date.	If-Modified-Since: Wed, 21 Oct 2023 07:28:00 GMT
	If-None-Match		Used with ETags to request a resource only if it has changed.	If-None-Match: "etag123"

	Special Headers for Specific Methods

	POST / PUT require Content-Type and Content-Length.

	DELETE may require authentication (Authorization header).

	HEAD is like GET but expects no response body.
*/

#include "../includes/Request.hpp"

void	Request::headersHostChecks( void ) const {
	int lines = _headers.count("host");
	if ( lines != 1 )
		throw headerException(errFlag::HEADER_HOST_INVALID);
	else if ( lines == 1)
	{
		str_t value = _headers.find("host")->second;
		if ( value.empty() || ( !_uri.host.empty() && value.compare(_uri.host) != 0) )
			throw headerException(errFlag::HEADER_HOST_INVALID);
	}
	return;
}

void	Request::headersTransferEncoding( str_t val){
	const std::set<str_t> valid_te = getHeaderTransferEncodings();

	if (valid_te.find(val) == valid_te.end())
		throw headerException(errFlag::	HEADER_TRANSFER_ENCODING_NOT_IMPLEMENTED);
	return ;
}

void	Request::headerCheck( const str_t header, void(*f)(str_t)){
	std::map< std::string, std::vector< std::string> >::iterator	it;

	it = _headers.find(header);
	if (it != _headers.end())
		f(it->second);
	return ;
}

bool	Request::validateHeaders( void ){

	try {
		if (_version.compare("HTTP/1.1") == 0 ){
			headersHostChecks();
		headerCheck("transfer-encoding", headersTransferEncoding);
		}
	}
	catch (uint64_t errnum){ return _error.addErrorFlag( errnum ); }
}

bool	Request::validateHeaders( void ){
	std::map< std::string, std::vector< std::string> >::iterator	it;

/*
	 Ensure host name matches server name 
	 Is there a scheme header? Ensure match with http, https
*/
{ // TRANSFER ENCODING & CONTENT LENGTH : ONLY ONE OF TWO PERMITTED : THIS IS A 'MAY' BUT CONNECTION 'MUST' BE CLOSED
	if ((it = _headers.find("transfer-encoding")) != _headers.end() && (it = _headers.find("content-length")) != _headers.end())
		return _error.addErrorFlag(errFlag::HEADER_MULTIPLE_LENGTH_HEADERS);
}

/*
	Other recipients SHOULD ignore unrecognized header and trailer fields.
-
	if (header key not recognised)
		ignore header
*/	
/*
	Other recipients SHOULD ignore unrecognized header and trailer fields.
-
	if (header key not recognised)
		ignore header
*/	
/*
	check which headers should only be single line
	for others, either combine to single entry as CSV's (order MUST not change
	Set-Cookie header is an exception to this rule
*/
/*
	A server MUST NOT apply a request to the target resource until it receives the entire request header section, 
	since later header field lines might include conditionals, authentication credentials, or deliberately misleading 
	duplicate header fields that could impact request processing
*/
/*
 	Look at field-length semantics for individual headers
*/
/*
	A server that receives a request header field line, field value, or set of fields larger than it wishes
	to process MUST respond with an appropriate 4xx (Client Error) status code. 
*/
/*
	A client MAY discard or truncate received field lines that are larger than the client wishes to process 
	if the field semantics are such that the dropped value(s) can be safely ignored
*/

/*	FIELD VALUE RULES */

/* 
	A field value does not include leading or trailing whitespace. When a specific version of HTTP allows such
	whitespace to appear in a message, a field parsing implementation MUST exclude 
	such whitespace prior to evaluating the field value.

	ChatGPT contradicts this : says some values must not be trimmed
*/

/*
	For defining field value syntax, this specification uses an ABNF rule named after the field name to define the
	allowed grammar for that field's value 	(after said value has been extracted from the underlying
	messaging syntax and multiple instances combined into a list
*/

/*
	VALID VALUES :

	- usually constrained to the range of US-ASCII characters
	- Fields needing a greater range of characters can use an encoding, such as the one defined in [RFC8187].
	- Historically, HTTP allowed field content with text in the ISO-8859-1 charset [ISO-8859-1],supporting other charsets only through use of [RFC2047] encoding 
	- Specifications for newly defined fields SHOULD limit their values to visible US-ASCII octets (VCHAR (i.e. visible)), SP, and HTAB.
	- A recipient SHOULD treat other allowed octets in field content (i.e., obs-text) as opaque data.

	- a recipient of CR, LF, or NUL within a field value MUST either reject the message or replace 
		each of those characters with SP before further processing or forwarding of that message.
	- Field values containing other CTL characters are also invalid; however, recipients MAY retain such characters for the sake of robustness when they appear 
		within a safe context (e.g., an application-specific quoted string that will not be processed by any downstream HTTP parser).

	- Fields that only anticipate a single member as the field value are referred to as "singleton fields"
	- Fields that allow multiple members as the field value are referred to as "list-based fields".
	- Fields that expect to contain a comma within a member, such as within an HTTP-date or URI-reference element, ought to be defined with delimiters 
		around that element to distinguish any comma within that data from potential list separators.
*/

	return true; 
}

uint64_t Request::headerException::error() const throw() { return _errnum; }