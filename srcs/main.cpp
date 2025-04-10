#include "../damianServer/Config.hpp"
#include "../damianServer/ConfigParser.hpp"
#include "../includes/Request.hpp"
#include "../includes/Response.hpp"
#include "../includes/StatusManager.hpp"

#include <stdlib.h>

int main(int ac, char**av) {

    try {
        /* default */
		if (ac != 2) {
			std::cerr << "Usage: " << av[0] << " <config_file>" << std::endl;
			return 1;
		}
		ConfigParser cp(av[1]);
		Config* config = cp.parseServerblocks();
	
		std::string	str = "GET https://user@example.com/path HTTP/1.1\r\n";
		Request		request(str, config);

        Response        response(request);

        StatusEntry e0 = response.getStatusManager().getStatusQueue().front();
        std::cout << e0 << std::endl;
        response.addStatusCode(400);
        StatusEntry e1 = response.getStatusManager().getStatusQueue().front();
        std::cout << e1 << std::endl;

        response.addStatusCode(403);
        StatusEntry e2 = response.getStatusManager().getStatusQueue().front();
        std::cout << e2 << std::endl;

        response.addStatusCode(409);
        StatusEntry e3 = response.getStatusManager().getStatusQueue().front();
        std::cout << e3 << std::endl;

        return (0);
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return (1);
    }
    
}
