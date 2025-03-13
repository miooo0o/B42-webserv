/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:56:17 by kmooney           #+#    #+#             */
/*   Updated: 2025/03/13 00:00:22 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"

void	test_function(const std::string& requestLine, const std::string& testLine, const std::string& purpose, std::string num )
{
	Request		request (requestLine);
	std::string testNum = "REQUEST LINE TEST : " + num + " : " + purpose;
	mainHeading(testNum, false, false, RY);
	textColoured(testLine, true, true, WB);
	std::cout << request;
}

int main()
{
	// test_function( "GET https://user@example.com/path HTTP/1.1\r\n",
	// 				"GET https://user@example.com/path HTTP/1.1\\r\\n",
	// 				" VALID REQUEST LINE ", "1");
	// test_function( "ABC https://user:pass@example.com HTTP/1.1\r\n",
	// 				"ABC https://user@example.com/path HTTP/1.1\\r\\n",
	// 				" ERROR IN METHOD - NOT RECOGNISED ", "2");
	// test_function( "get https://user@example.com/path HTTP/1.1\r\n",
	// 				"get https://user@example.com/path HTTP/1.1\\r\\n",
	// 				" ERROR IN METHOD - LOWER CASE ", "3");
	// test_function( "GET https://user:pass@example.com HTTP/0.9\r\n",
	// 				"GET https://user@example.com/path HTTP/1.2\\r\\n",
	// 				" ERROR IN VERSION NUMBER - NOT SUPPORTED ", "4");
	// test_function( "GET https://user:pass@example.com HTTP/1.2\r\n",
	// 				"GET https://user@example.com/path HTTP/1.2\\r\\n",
	// 				" ERROR IN VERSION NUMBER - NOT RECOGNISED ", "5");
	// test_function( "GET https%%25~://user:pass@example.com HTTP/1.1\r\n",
	// 				"GET https%%25~://user:pass@example.com HTTP/1.1\\r\\n",
	// 				" ERROR IN URI SCHEME - INVALID CHARS %%~ ", "6");	
	test_function( "GET https://user@example.%63om/path HTTP/1.1\r\n",
					"GET https://user@example.%63om/path HTTP/1.1\\r\\n",
					" URI PATH - PERCENT ENCODING REPLACEMENT TEST - %63 = c ", "7");
	test_function( "GET https://user@example.%2563om/path HTTP/1.1\r\n",
					"GET https://user@example.%2563om/path HTTP/1.1\\r\\n",
					" URI PATH - PERCENT ENCODING REPLACEMENT TEST - %25 = % ", "8");
	test_function( "GET https://%EC%95%88%EB%85%95%20%EB%AF%B8%EB%82%98/path HTTP/1.1\r\n",
					"GET https://%EC%95%88%EB%85%95%20%EB%AF%B8%EB%82%98/path HTTP/1.1\\r\\n",
					" URI PATH - PERCENT ENCODING UTF8 TEST", "9");
	test_function( "GET https://%EC%A3%BC%EC%9A%94%20%EC%ZZ%B8%EA%B5%90%ED%99%9C%EB%8F%99/path HTTP/1.1\r\n",
					"GET https://%EC%A3%BC%EC%9A%94%20%EC%ZZ%B8%EA%B5%90%ED%99%9C%EB%8F%99/path HTTP/1.1\\r\\n",
					" URI PATH - PERCENT ENCODING UTF8 FAILED MAJOR DIPLOMATIC ACTIVITIES", "10");

	test_function( "GET ftp://example.com/path HTTP/1.1\r\n",
					"GET ftp://user@example.com/path HTTP/1.1\\r\\n",
					" URI SCHEME NOT SUPPORTED ", "11");
					
/* 	{
		Request request("GET https://example.com HTTP/1.1\r\n");
		mainHeading("REQUEST LINE TEST", false, false, RY);
		textColoured("GET https://example.com HTTP/1.1\\r\\n", true, true, WB);
		std::cout << request;
	}
	{
		Request request("GET https://user@example.com:8080 HTTP/1.1\r\n");
		mainHeading("REQUEST LINE TEST", false, false, RY);
		textColoured("GET https://user@example.com:8080 HTTP/1.1\\r\\n", true, true, WB);
		std::cout << request;
	}
	{
		Request request("GET https://user@example.com/path HTTP/1.1\r\n");
		mainHeading("REQUEST LINE TEST", false, false, RY);
		textColoured("GET https://user@example.com/path HTTP/1.1\\r\\n", true, true, WB);
		std::cout << request;
	}
	{
		Request request("DELETE https://user@example.com/path/abc/def/123.com HTTP/1.1\r\n");
		mainHeading("REQUEST LINE TEST", false, false, RY);
		textColoured("DELETE https://user@example.com/path/abc/def/123.com HTTP/1.1\\r\\n", true, true, WB);
		std::cout << request;
	}
	{
		Request request("GET http://www.w3.org/pub/WWW/TheProject.html HTTP/1.1\r\n");
		mainHeading("REQUEST LINE TEST", false, false, RY);
		textColoured(" GET http://www.w3.org/pub/WWW/TheProject.html HTTP/1.1\\r\\n", true, true, WB);
		std::cout << request;
	} */
}
