/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestValidator.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 01:37:52 by kmooney           #+#    #+#             */
/*   Updated: 2025/05/15 17:53:49 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_VALIDATOR_HPP
#define REQUEST_VALIDATOR_HPP

#include "RequestTypes.hpp"
#include "core_def.hpp"
#include "utility_functions.hpp"

class Request;
class RequestParser;

enum HeaderFlag {
	HDR_MANDATORY        = 1 << 1,
	HDR_ONE_INST_ONLY 	 = 1 << 2,
	HDR_ONE_VAL_ONLY	 = 1 << 3,
	HDR_CAN_IGNORE       = 1 << 4,
	HDR_CRITICAL_ERROR   = 1 << 6,
	HDR_VALUE_CHECK      = 1 << 6,
	HDR_OPTION_CHECK     = 1 << 7,
	HDR_SERVER_CAPABLE   = 1 << 8
};

typedef struct HeaderFlags {
	const char*	name;
	int			flags;
} HeaderFlags_t;

class RequestValidator {
	
	public:
							RequestValidator(Request* request);
							~RequestValidator();

		void 				validate();

	private:

		Request*					_req;
		std::map<std::string, int>	hdr_seen;
		const HeaderFlags*			_hdr_flags;

		void				validateReqLine();
		bool				validateMethod();
		bool				validateURI();
		bool				validateScheme();
		bool				validateUser();
		bool				validatePass();
		bool				validateHost();
		bool				validatePort();
		bool				validatePath();
		bool				validateQuery();
		bool				validateFrag();
		bool				validateVersion();

		void				validateHeaders();
		bool				validateHostHeader();

		bool				validateRepresentation();
		bool				identifyLengthHeader();
		bool				validateTransferEncoding();
		bool				validateContentLength();
		bool				convertContentLength();
		bool				validateContentEncoding();

		bool				validateContentType();
		bool				validateMultiPart(const std::string& val, const std::vector< std::pair<std::string, std::string> >& options);
		bool				validateCharset(const std::string& val, const std::vector< std::pair<std::string, std::string> >& options);
		
		bool				duplicateOptionCheck(const std::vector< std::pair<std::string, std::string> >& options);
		bool				duplicateValueCheck(const std::vector<HeaderField_t*>& values);
		
		bool				validate_all_headers(const headerMap_t& headers);
		bool				validate_header(const std::string& name, const std::vector<HeaderField_t*>& fields, const HeaderFlags* meta);
		void				headerDefinitions();
	
		const HeaderFlags*	find_header_flags(const std::string& name);
		int					compute_received_flags(const std::string& name, std::vector<HeaderField_t*> values);
		
		bool				validateMethodHeaders();
		
		void				validateBody();
		
		int					countHeaderValues (const headerMap_t& headers, const std::string& headerName);
		int					countOptions(const std::vector<std::pair<std::string, std::string> >& options);
		bool				isTokenChar(char c);
		bool				isToken(const std::string& s);
		bool				percentDecode( str_t& encoded);
		bool				uriCharValidation(const str_t set, const str_t& target);
};

bool	temporaryIsValidContentType(const std::string& name);


#endif