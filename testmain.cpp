#include <iostream>
#include <string>

void parseAuthHeader(const std::string& header) {
    size_t pos = header.find(" ");
    if (pos != std::string::npos) {
        std::string scheme = header.substr(0, pos);
        std::string credentials = header.substr(pos + 1); // Preserve spaces in credentials

        std::cout << "Auth Scheme: [" << scheme << "]\n";
        std::cout << "Credentials: [" << credentials << "]\n";
    }
}

int main() {
    std::string authHeader = "Bearer  abc123  "; // Spaces matter in some cases
    parseAuthHeader(authHeader);

    return 0;
}
