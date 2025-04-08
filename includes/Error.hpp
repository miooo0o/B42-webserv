/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                               	        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:57:35 by kmooney           #+#    #+#             */
/*   Updated: 2025/03/19 14:26:11 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HPP
# define ERROR_HPP

#include "./core_def.hpp"
#include "./TestClasses/testUtils.hpp"
#include "./utility_functions.hpp"
#include "./Logs.hpp"

#include <cstring>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <fstream>
#include <ctime>

#define CODE100 "Continue"
#define CODE101 "Switching Protocols"
#define CODE102 "Processing"
#define CODE103 "Early Hints"
#define CODE200 "OK"
#define CODE201 "Created"
#define CODE202 "Accepted"
#define CODE203 "Non-Authoritative Information"
#define CODE204 "No Content"
#define CODE205 "Reset Content"
#define CODE206 "Partial Content"
#define CODE207 "Multi-Status"
#define CODE208 "Already Reported"
#define CODE226 "IM Used"
#define CODE300 "Multiple Choices"
#define CODE301 "Moved Permanently"
#define CODE302 "Found"
#define CODE303 "See Other"
#define CODE304 "Not Modified"
#define CODE305 "Use Proxy"
#define CODE306 "Switch Proxy"
#define CODE307 "Temporary Redirect"
#define CODE308 "Permanent Redirect"
#define CODE400 "Bad Request"
#define CODE401 "Unauthorized"
#define CODE402 "Payment Required"
#define CODE403 "Forbidden"
#define CODE404 "Not Found"
#define CODE405 "Method Not Allowed"
#define CODE406 "Not Acceptable"
#define CODE407 "Proxy Authentication Required"
#define CODE408 "Request Timeout"
#define CODE409 "Conflict"
#define CODE410 "Gone"
#define CODE411 "Length Required"
#define CODE412 "Precondition Failed"
#define CODE413 "Payload Too Large"
#define CODE414 "URI Too Long"
#define CODE415 "Unsupported Media Type"
#define CODE416 "Range Not Satisfiable"
#define CODE417 "Expectation Failed"
#define CODE418 "I'm a teapot"
#define CODE421 "Misdirected Request"
#define CODE422 "Unprocessable Content"
#define CODE423 "Locked"
#define CODE424 "Failed Dependency"
#define CODE425 "Too Early"
#define CODE426 "Upgrade Required"
#define CODE428 "Precondition Required"
#define CODE429 "Too Many Requests"
#define CODE431 "Request Header Fields Too Large"
#define CODE451 "Unavailable For Legal Reasons"
#define CODE500 "Internal Server Error"
#define CODE501 "Not Implemented"
#define CODE502 "Bad Gateway"
#define CODE503 "Service Unavailable"
#define CODE504 "Gateway Timeout"
#define CODE505 "HTTP Version Not Supported"
#define CODE506 "Variant Also Negotiates"
#define CODE507 "Insufficient Storage"
#define CODE508 "Loop Detected"
#define CODE510 "Not Extended"
#define CODE511 "Network Authentication Required"

#define NO_ERROR			"No error"
#define HEAD_CONT_UNS		"415 Headers : Content-Type : Unsupported media type"
#define HEAD_END			"400 Headers: Incorrect character sequence at end of headers"
#define HEAD_OMI_CONT		"415 Headers : Content-Type : Omitted"
#define HEAD_HOST			"400 Headers : Host : error in Host header (omitted, multiple lines or incorrect format) "
#define HEAD_SEP			"400 Headers: entries must be separated by \\r\\n"
#define METH_NOT_PERM		"405 Method : Method not permitted for the target resource"
#define METH_UNREC			"501 Method : Method not recognised" // REMOVE
#define METH_UNSUP			"501 Method : Method not supported"
#define METH_CASE			"400 Method : Invalid Format: must be uppercase"
#define REQ_END				"400 Request Line : Incorrect character sequence at end of request line"
#define URI_ENCOD			"400 URI : Invalid Percent Encoding"
#define URI_FRAG_REC		"400 URI : Fragment identifier (#) is not allowed in request URI"
#define URI_HOST_RES		"404 URI : Host : could not be resolved"
#define URI_ILLEGAL_CHARS	"400 URI : Illegal characters"
#define URI_LENGTH			"400 URI : Requested URI is too long to process"
#define URI_LOOP			"400 URI : Too many URI redirections"
#define URI_PATH_INV		"400 URI : Path is invalid : "
#define URI_PATH_LEN		"400 URI : Path : too long"
#define URI_PATH_TRAV		"403 URI : Path : Attempted directory traversal detected"
#define URI_PATH_UNSAFE		"400 URI : Path : Unsafe Characters in Path"
#define URI_PORT_INV		"400 URI : Port : Invalid or missing port number in URI"
#define URI_QUERY_CONF		"400 URI : Query : Conflicting parameters in request"
#define URI_QUERY_INV		"400 URI : Query : Invalid Query String"
#define URI_SCH_UNSUP		"400 URI : Scheme not supported"
#define URI_SCH_UNREC		"400 URI : Scheme not recognised"
#define VER_NONE			"400 Version : HTTP version not provided"
#define VER_UNREC			"400 Version : Unrecognised HTTP Version"
#define VER_UNSUP			"400 Version : Unsupported HTTP Version"

#define HEAD_HOST_MIS		"400 Headers : Host : Mismatch" // NOT SURE IF REQUIRED FOR US
#define HEADERS_MULTI_LEN	"501 Headers: Headers : Content-Length and Transfer-Encoding Headers present : only one length header permitted"
#define HEADERS_EMPTY_VAL	"501 Headers: Headers : Content-Length and Transfer-Encoding Headers present : only one length header permitted"
#define	HEADER_TR_ENCOD_NOT	"501 Headers: Headers : Content-Length and Transfer-Encoding Headers present : only one length header permitted"
/*
TO BE ADDED 
 */

struct errFlag{
	static const uint64_t NONE = 0;
	static const uint64_t HEADER_CONTENT_UNSUPPORTED = 1ULL << 1;
	static const uint64_t HEADER_END = 1ULL << 2;
	static const uint64_t HEADER_OMITTED_CONTENT_TYPE = 1ULL << 3;
	static const uint64_t HEADER_HOST_INVALID = 1ULL << 4;
	static const uint64_t HEADER_SEPARATOR = 1ULL << 5;
	static const uint64_t METHOD_NOT_PERMITTED = 1ULL << 6;
	static const uint64_t METHOD_UNRECOGNISED = 1ULL << 7;
	static const uint64_t METHOD_UNSUPPORTED = 1ULL << 8;
	static const uint64_t METHOD_CASE = 1ULL << 9;
	static const uint64_t REQUEST_ENDING = 1ULL << 10;
	static const uint64_t URI_ENCODING = 1ULL << 11;
	static const uint64_t URI_FRAG_RECEIVED = 1ULL << 12;
	static const uint64_t URI_HOST_RESOLUTION_FAILED = 1ULL << 13;
	static const uint64_t URI_ILLEGAL_CHARACTER = 1ULL << 14;
	static const uint64_t URI_LENGTH_TOO_LONG = 1ULL << 15;
	static const uint64_t URI_LOOP_TOO_MANY_REDIR = 1ULL << 16;
	static const uint64_t URI_PATH_INVALID	= 1ULL << 17;
	static const uint64_t URI_PATH_TRAVERSAL = 1ULL << 18;
	static const uint64_t URI_PATH_IS_UNSAFE = 1ULL << 19;
	static const uint64_t URI_PATH_LENGTH = 1ULL << 20;
	static const uint64_t URI_PORT_INVALID = 1ULL << 21;
	static const uint64_t URI_QUERY_CONFLICT_PARAM = 1ULL << 22;
	static const uint64_t URI_QUERY_INVALID = 1ULL << 23;
	static const uint64_t URI_SCHEME_UNSUPPORTED = 1ULL << 24;
	static const uint64_t URI_SCHEME_UNRECOGNISED = 1ULL << 25;
	static const uint64_t VERSION_NONE= 1ULL << 26;
	static const uint64_t VERSION_UNRECOGNISED = 1ULL << 27;
	static const uint64_t VERSION_UNSUPPORTED = 1ULL << 28;

	static const uint64_t HEADER_HOST_MISMATCH = 1ULL << 29; // NOT SURE IF REQUIRED
	static const uint64_t HEADER_MULTIPLE_LENGTH_HEADERS = 1ULL << 30;
	static const uint64_t HEADER_EMPTY_VALUE = 1ULL << 31;
	static const uint64_t HEADER_TRANSFER_ENCODING_NOT_IMPLEMENTED = 1ULL << 32;
};

class	Error {
	
	std::vector<std::string>	_flagStrings;
	
	uint64_t			_lastError;
	uint64_t			_errorFlags;
	int					_response_code;
	int					_last_response_code;
	bool				isFlagSet(uint64_t flag);
	
	Logs*				_error_log;

	public:
	
	/* CONSTRUCTORS */
	Error( void );
	Error( uint64_t error);
	Error( const Error& other );
	
	/* DESTRUCTORS */
	~Error( void );
	
	/* COPY ASSIGNMENT */
	Error&			operator=(const Error& other);
	
	void			flagStringsInit();
	
	/* SETTERS */
	bool			addErrorFlag(uint64_t error);
	
	/* GETTERS */	
	std::string		getErrorNum(int pos);
	std::string		getErrorStr1(int pos);
	std::ostream&	getErrorInfo(int pos, std::ostream& os);
	std::ostream&	getErrorStream(std::ostream& os);
	int				getLastResponseCode();
	int				getPosition(uint64_t num);
	void			logError();
	//void			printErrors(std::ostream& os) const;
	//std::string&	getErrorStr2(std::ostream& os);
	//std::string	getErrorStringFromNumber(int code);
	//void			getError();
};  

/* OUTPUT FORMAT  */
std::ostream& l14(std::ostream& os);
std::ostream& r3(std::ostream& os);

/* PRINTING REQUEST */
std::ostream&	operator<<(std::ostream& os, Error& error);
#endif
