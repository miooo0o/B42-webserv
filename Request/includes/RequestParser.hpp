/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 22:01:11 by kmooney           #+#    #+#             */
/*   Updated: 2025/05/13 11:05:19 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_PARSER_HPP
# define REQUEST_PARSER_HPP

#include "RequestTypes.hpp"
#include "core_def.hpp"
#include "utility_functions.hpp"

class Request;

#define SPACE " "
#define LINE "\r\n"
#define FSLASH "/"
#define COLON ":"
#define DELIM_REQUIRED true
#define DELIM_OPTIONAL false

/* PARSING STATE ENUMS */
enum e_reqline { RL_LINE, RL_METHOD, RL_URI, RL_VERSION, RL_COMPLETE };

typedef struct token_s {

	std::deque<char>::const_iterator	begin;
	std::deque<char>::const_iterator	end;
	
	token_s() {}
	token_s(std::deque<char>::const_iterator b, std::deque<char>::const_iterator e)
		: begin(b), end(e) {}

} token_t;

class RequestParser {
	public:
					RequestParser(Request* request);
					~RequestParser();

	void			parse(const char* data, size_t length, Request& request);
	void			addData(const char* data, size_t length);
	void			parseRequest();

	void			parseReqLine(token_t& token);
	bool			getRequestLine(token_t& rl, token_t& it);
	
	bool			parseURI( token_t& rl );
	void			parseScheme( states& state, token_t& uri );
	void			parseUser( states& state, token_t& uri );
	void			parseHost( states& state, token_t& uri );
	void			parsePort( states& state, token_t& uri );
	void			parsePath( states& state, token_t& uri );
	void			parseQuery( states& state, token_t& uri );
	void			parseFrag( states& state, token_t& uri );
	bool			URInormalisation( void );

	void			parseHeaders(token_t& it);
	bool			parseHeaderLine(token_t& line);
	bool			parseHeaderVal(token_t& headerTkn, HeaderField_t* splitHeaderVal);

	void			parseBody(token_t& it);
	void			parsePlain(token_t& it);
	bool			parseChunkData(token_t& it, std::string& data);
	bool			parseChunkSize(token_t& it);
	void			parseChunked(token_t& it);
	void			parseMultiPart(token_t& it);

	void			tokenToString(const token_t& token, std::string& dest);
	std::string		tokenToString(const token_t& token, std::string& dest, int debug);

	bool			parseSingleToken(token_t& it, bool last_val, std::string& dest, const std::string& delim, bool delim_req);
	bool			parseNextLine(token_t& it, token_t& line);

	size_t			splitToToken(token_t& it, bool last_val, std::vector<token_t>& tkns, const std::string& delim);
	bool			splitToToken(token_t& it, bool last_val, token_t& tkn, const std::string& delim);
	bool			splitToTokenMultiDelims(token_t& it, bool last_val, token_t& tkn, const std::string& delims, char& found_delim);

	size_t			getUnparsedLength();

	template <class T>
	void			setIterators(token_t& t, T& begin, T& end);
	void			setIterators();

private:
	
	Request*				 _req;
	
	std::deque<char>		_buffer;
	std::vector<token_t>	_tknLst;
	token_t					_p_it;
	
	int						_rl_state;

	size_t					_unparsed_len;
	size_t					_expected_len;
	size_t					_parsed_body_len;
	uint64_t				_chunk_size;
};

#endif
