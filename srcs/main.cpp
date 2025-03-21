#include "Request.hpp"
#include "Response.hpp"
#include "Entries.hpp"

#include <stdlib.h>

int main(int ac, char**av) {
    if (ac != 2)
        return (1);
    
    try {
        /* default */
        int test_code = std::atoi(av[1]);
        Request     request(test_code);
        Entries     entries(request);
        Response    response(request, &entries);

        std::cout << "== First Entry ==" << std::endl;
        std::cout << "Request Code: " << request.getCode() << std::endl;
        std::cout << "Entry Code: " << entries.getEntry().getCode() << std::endl;
        std::cout << "Class: " << static_cast<int>(entries.getClass()) << std::endl;
        std::cout << "Validation Status: " << static_cast<int>(entries.getEntry().getValidateStatus()) << std::endl;
        std::cout << std::endl;

        return (0);
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return (1);
    }
    
}