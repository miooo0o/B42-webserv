#include <iostream>
#include <cassert>
#include "../includes/Request.hpp"
#include <cstring>

void test_parse_method(std::string& str, const std::string& methodStr, int num, bool test, int expected_code){

	Request req(str);

	assert(req.getMethodString() == methodStr);
	if (test == true){
		assert(req.getResponseCode() == 200);
		std::cout << "test_parse_method " << num << " passed : method valid\t: " << methodStr << std::endl;
	}
	else {
		assert(req.getResponseCode() == 400);
		std::cout << "test_parse_method " << num << " passed : method invalid\t: " << methodStr << std::endl;
	}
}

int main() {
	std::string	request0 = "GET https://user@example.com/path HTTP/1.1\r\n";
	std::string	request1 = "GEt https://user@example.com/path HTTP/1.1\r\n";
	std::string	request2 = "GeT https://user@example.com/path HTTP/1.1\r\n";
	std::string	request3 = "POST https://user@example.com/path HTTP/1.1\r\n";
	std::string	request4 = "Post https://user@example.com/path HTTP/1.1\r\n";
	std::string	request5 = "DELETE https://user@example.com/path HTTP/1.1\r\n";
	std::string	request6 = "Delete https://user@example.com/path HTTP/1.1\r\n";
	std::string	request7 = "MADEUP https://user@example.com/path HTTP/1.1\r\n";
	std::string	request8 = "PUT https://user@example.com/path HTTP/1.1\r\n";

    test_parse_method(request0, "GET", 0, true, 200);
    test_parse_method(request1, "GEt", 1, false, 400);
    test_parse_method(request2, "GeT", 2, false, 400);
    test_parse_method(request3, "POST", 3, true, 400);
    test_parse_method(request4, "Post", 4, false, 400);
	test_parse_method(request5, "DELETE", 5, true, 400);
    test_parse_method(request6, "Delete", 6, false, 400);
    test_parse_method(request7, "MADEUP", 7, false, 501);
    test_parse_method(request8, "PUT", 8, false, 501);

    return 0;
}