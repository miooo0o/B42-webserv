#include <iostream>
#include <cassert>
#include "../../../includes/Request.hpp"
#include <cstring>

void test_parse_version(std::string& str, const std::string& versionStr, int num, bool test){

	Request req(str);

	assert(req.getVersionString() == versionStr);
	if (test == true){
		assert(req.getResponseCode() == 200);
		std::cout << "test_parse_version " << num << " passed : version valid\t: " << versionStr << std::endl;
	}
	else {
		assert(req.getResponseCode() == 400);
		std::cout << "test_parse_version " << num << " passed : version invalid\t: " << versionStr << std::endl;
	}
}

int main() {
	std::string	request0 = "GET https://user@example.com/path HTTP/1.2\r\n";
	std::string	request1 = "GET https://user@example.com/path HTTP/1.1\r\n";
	std::string	request2 = "GET https://user@example.com/path HTTP/1.0\r\n";
	std::string	request3 = "GET https://user@example.com/path HTTP/2.0\r\n";
	std::string	request4 = "GET https://user@example.com/path HTTP/3.0\r\n";
	std::string	request5 = "GET https://user@example.com/path HTP/1.1\r\n";
	std::string	request6 = "GET https://user@example.com/path HTTP1.2\r\n";
	std::string	request7 = "GET https://user@example.com/path HTTP/1.1 \r\n";

    test_parse_version(request0, "HTTP/1.2", 0, false);
    test_parse_version(request1, "HTTP/1.1", 1, true);
    test_parse_version(request2, "HTTP/1.0", 2, true);
    test_parse_version(request3, "HTTP/2.0", 3, false);
    test_parse_version(request4, "HTTP/3.0", 4, false);
	test_parse_version(request5, "HTP/1.1", 5, false);
    test_parse_version(request6, "HTTP1.2", 6, false);
    test_parse_version(request7, "HTTP/1.1 ", 7, false);

    return 0;
}