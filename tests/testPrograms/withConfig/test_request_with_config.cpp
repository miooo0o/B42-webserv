/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_request_with_config.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:56:17 by kmooney           #+#    #+#             */
/*   Updated: 2025/04/08 12:50:34 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Request.hpp"
#include "../../../damianServer/ConfigParser.hpp"
#include "../../../damianServer/Config.hpp"
#include "../../../includes/TestClasses/testUtils.hpp"
#include <cstring>

std::string	escapeChars(const std::string& str){
	std::string result;
	
	for (size_t i = 0; i < str.length(); i++) {
		switch(str[i]){
			case '\n'	:
				result += "\\n";
				break;
			case '\r'	:
				result += "\\r";
				break;
			default		:
				result += str[i];
				break;
		}
	}
	return result;
}

void	test_function(const std::string& requestLine, const std::string& purpose, std::string num, Config *config )
{
	Request		request (requestLine, config);
	std::string line = escapeChars(requestLine);
	std::string testNum = "REQUEST LINE TEST : " + num + " : " + purpose;
	mainHeading(testNum, false, false, RY);
	textColoured(line, true, true, WB);
	std::cout << request;
}

int	main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
		return 1;
	}
	ConfigParser cp(argv[1]);
	Config* config = cp.parseServerblocks();
	
	std::cout << config << std::endl;
	
	// /* METHOD TESTS */
	test_function( "GET /myFork/damianServer HTTP/1.1\r\n", " PERMITTED METHOD, VALID PATH ", "1", config);
	test_function( "GET /myFork/ HTTP/1.1\r\n", " PERMITTED METHOD, SHORT PATH ", "2", config);
	test_function( "GET /tests/ HTTP/1.1\r\n", " PERMITTED METHOD, INVALID PATH ", "3", config);
	test_function( "GET / HTTP/1.1\r\n", " PERMITTED METHOD, NO PATH ", "4", config);

	test_function( "POST /test/test.html HTTP/1.1\r\n", " PERMITTED METHOD, VALID PATH ", "1", config);
	test_function( "POST /test/ HTTP/1.1\r\n", " PERMITTED METHOD, SHORT PATH ", "1", config);
	test_function( "POST /tests/ HTTP/1.1\r\n", " PERMITTED METHOD, INVALID PATH ", "3", config);
	test_function( "POST / HTTP/1.1\r\n", " PERMITTED METHOD, NO PATH ", "4", config);

	test_function( "DELETE /test/test.html HTTP/1.1\r\n", " PERMITTED METHOD, VALID PATH ", "1", config);
	test_function( "DELETE /test/ HTTP/1.1\r\n", " PERMITTED METHOD, SHORT PATH ", "1", config);
	test_function( "DELETE /tests/ HTTP/1.1\r\n", " PERMITTED METHOD, INVALID PATH ", "3", config);
	test_function( "DELETE / HTTP/1.1\r\n", " METHOD : VALID GET, NO PATH ", "4", config);


	test_function( "POST /test/test.html HTTP/1.1\r\n",	" METHOD : VALID POST ", "2", config);
	test_function( "DELETE /test/test.html HTTP/1.1\r\n", " METHOD : VALID DELETE ", "3", config);
	// test_function( "DELETE https://user@example.com/path/abc/def/123.com HTTP/1.1\r\n",
	// 	" METHOD : VALID DELETE ", "2", config);
	// test_function( "POST https://user@example.com/path/abc/def/123.com HTTP/1.1\r\n",
	// 	" METHOD : VALID POST ", "3", config);
	// test_function( "ABC https://user:pass@example.com HTTP/1.1\r\n",			
	// 	" METHOD : ERROR : UNRECOGNISED ", "4", config);
	// test_function( "get https://user@example.com/path HTTP/1.1\r\n",
	// 	" METHOD : ERROR : LOWER CASE ", "5", config);
	// test_function( "PUT https://user@example.com/path HTTP/1.1\r\n",
	// 	" METHOD : ERROR : UNSUPPORTED ", "6", config);
		
	// /* VERSION TESTS */		
	// test_function( "GET https://user:pass@example.com HTTP/0.9\r\n",
	// 	" VERSION : ERROR : NOT SUPPORTED ", "7", config);
	// test_function( "GET https://user:pass@example.com HTTP/1.2\r\n",
	// 	" VERSION : ERROR : UNRECOGNISED ", "8", config);
	// test_function( "GET https://user:pass@example.com HTTP/1.2\r\n",
	// 	" VERSION : ERROR : NOT RECOGNISED ", "9", config);

	// /* URI TESTS */			
	// test_function( "GET https%%25~://user:pass@example.com HTTP/1.1\r\n",
	// 	" URI : SCHEME : INVALID CHARS %%~ ", "10", config);	
	// test_function( "GET ftp://example.com/path HTTP/1.1\r\n",
	// 	" URI : SCHEME : NOT SUPPORTED ", "11", config);
	// test_function( "GET https://user@example.%63om/path HTTP/1.1\r\n",
	// 	" URI : PATH : PERCENT ENCODING REPLACEMENT TEST - %63 = c ", "12", config);
	// test_function( "GET https://user@example.%2563om/path HTTP/1.1\r\n",
	// 	" URI : PATH : PERCENT ENCODING REPLACEMENT TEST - %25 = % ", "13", config);
	// test_function( "GET https://%EC%95%88%EB%85%95%20%EB%AF%B8%EB%82%98/path HTTP/1.1\r\n",
	// 	" URI : PATH : PERCENT ENCODING UTF8 TEST", "14", config);
	// test_function( "GET https://%EC%A3%BC%EC%9A%94%20%EC%ZZ%B8%EA%B5%90%ED%99%9C%EB%8F%99/path HTTP/1.1\r\n",
	// 	" URI : PATH : PERCENT ENCODING UTF8 FAILED ", "15", config);
	
	// /* LINE ENDINGS */
	// test_function( "GET https://user@example.com:8080 HTTP/1.1\r", " REQUEST LINE : BAD ENDING (\\n missing) ", "11", config);
	// test_function( "GET https://user@example.com:8080 HTTP/1.1\n", " REQUEST LINE : BAD ENDING (\\r missing) ", "12", config);	

	// /* QUERY TESTS */
	// test_function( "GET /search?a=query&b=terms&c=for&d=testing HTTP/1.1\n", "URI : QUERY", "13", config);

	// /* MULTIPLE ERROR TESTS */
	// test_function( "GET /search?a=query&b=terms&c=for&d=testing HTP/0.1\n", "MULTIPLE ERRORS", "14", config);

	// /*
	// test_function( , "", "");
	// test_function( , "", "");
	// */

}
