#include "Request.hpp"
#include "Response.hpp"
#include "StatusManager.hpp"

#include <stdlib.h>

int main(int ac, char**av) {
    if (ac != 2)
        return (1);
    
    try {
        /* default */
        int test_code = std::atoi(av[1]);
        Request         request(test_code);

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
