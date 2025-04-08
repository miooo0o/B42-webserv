#include <iostream>
#include <cassert>
#include "../../../includes/Request.hpp"
#include "../../../damianServer/Config.hpp"
#include "../../../damianServer/ConfigParser.hpp"
#include <cstring>

void test_parse_method(Config* config, const std::string& methodStr, int num, int expected_code, bool test, const std::string& str){

	Request req(str, config);
	
	assert(req.getMethodString() == methodStr);
	if (test == true && expected_code == 200){
		assert(req.getResponseCode() == 200);
		std::cout << "test_parse_method " << num << " passed : method valid\t: " << methodStr << std::endl;
	}
	else {
		assert(req.getResponseCode() == expected_code);
		std::cout << "test_parse_method " << num << " passed : method invalid\t: " << methodStr << std::endl;
	}
}

int	main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
		return 1;
	}
	ConfigParser cp(argv[1]);
	Config* config = cp.parseServerblocks();

	/* GET TESTS */
    test_parse_method(config, "GET", 0, 200, true, "GET /path HTTP/1.1\r\n"); // Method valid and in route
    test_parse_method(config, "GEt", 1, 400, false, "GEt /path HTTP/1.1\r\n"); //Case invalid
    test_parse_method(config, "GeT", 2, 400, false, "GeT /path HTTP/1.1\r\n"); //Case invalid
	
	/* POST TESTS */
	test_parse_method(config, "POST", 3, 200, true, "POST https://example.com/path HTTP/1.1\r\n"); // Method valid and in route
    test_parse_method(config, "Post", 4, 400, false, "Post https://example.com/path HTTP/1.1\r\n"); //Case invalid
	
	/* DELETE TESTS */
	test_parse_method(config, "DELETE", 5, 200, true, "DELETE https://example.com/path HTTP/1.1\r\n"); //Method Valid and in route
    test_parse_method(config, "Delete", 6, 400, false, "Delete https://example.com/path HTTP/1.1\r\n"); // Case invalid
	
	/* NON-EXISTENT METHOD TESTS */
	test_parse_method(config, "MADEUP", 7, 501, false, "MADEUP https://example.com/path HTTP/1.1\r\n"); // Method not recognised
	
	/* UNSUPPORTED ON SERVER METHOD TESTS */
	test_parse_method(config, "PUT", 8, 501, false, "PUT https://example.com/path HTTP/1.1\r\n"); // Method exists but not recognised
	
	/* SUPPORTED ON SERVER METHOD TESTS */
	test_parse_method(config, "GET", 9, 405, false, "GET /info.html HTTP/1.1\r\n"); // Method supported on server but not in route

    return 0;
}