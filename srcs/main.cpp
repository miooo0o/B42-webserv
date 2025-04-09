#include "Request.hpp"
#include "Response.hpp"
#include "StatusManager.hpp"

#include <stdlib.h>

int main(int ac, char**av) {
    if (ac != 1)
        return (1);
    try {

        return (0);
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return (1);
    }
}