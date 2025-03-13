#include <iostream>
#include <cassert>
#include "../includes/Request.hpp"
#include <cstring>


void test_parse_method(std::string& str, const std::string& methodStr, int num, bool test){

	Request req(str);

	assert(req.getMethodString() == methodStr);
	if (test == true){
		assert(req.getResponseCode() == 200);
		std::cout << "test_parse_http_request " << num << " passed!" << std::endl;
	}
	else {
		assert(req.getResponseCode() == 400);
		assert(req.getLastErrorLoc() == "REQUEST LINE : METHOD");
		std::cout << "test_parse_http_request " << num << " passed!" << std::endl;
	}
}

int main() {
	std::string	request0 = "GET https://user@example.%63om/path HTTP/1.1\r\n";
	std::string	request1 = "GEt https://user@example.%63om/path HTTP/1.1\r\n";
	std::string	request2 = "GeT https://user@example.%63om/path HTTP/1.1\r\n";
	std::string	request3 = "gET https://user@example.%63om/path HTTP/1.1\r\n";
	std::string	request4 = "geT https://user@example.%63om/path HTTP/1.1\r\n";
	std::string	request5 = "GEt https://user@example.%63om/path HTTP/1.1\r\n";
	std::string	request6 = "geT https://user@example.%63om/path HTTP/1.1\r\n";

	std::string	request7 = "POST https://user@example.%63om/path HTTP/1.1\r\n";
	std::string	request8 = "Post https://user@example.%63om/path HTTP/1.1\r\n";
	
	std::string	request9 = "DELETE https://user@example.%63om/path HTTP/1.1\r\n";
	std::string	request10 = "Delete https://user@example.%63om/path HTTP/1.1\r\n";

    test_parse_method(request0, "GET", 0, true);
    test_parse_method(request1, "GEt", 1, false);
    test_parse_method(request2, "GeT", 2, false);
    test_parse_method(request3, "gET", 3, false);
    test_parse_method(request4, "geT", 4, false);
    test_parse_method(request5, "GEt", 5, false);
    test_parse_method(request6, "geT", 6, false);

    test_parse_method(request7, "POST", 7, true);
    test_parse_method(request8, "Post", 8, false);
    
	test_parse_method(request9, "DELETE", 9, true);
    test_parse_method(request10, "Delete", 10, false);

    return 0;
}