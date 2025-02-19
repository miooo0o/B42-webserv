#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include "Config.hpp"
#include "Route.hpp"

class ConfigParser {
public:

static void parseLocation(Config& config, std::ifstream& file, std::string& oldLine)
{
    std::string line;
    Route* currentRoute = new Route();

    currentRoute->setPath(parseLocationPath(oldLine));
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.find("autoindex") == 0) {
            currentRoute->setAutoindex(parseValue<std::string>(line));
        }
        else if (line.find("return") == 0) {
            std::istringstream iss(line);
            std::string key;
            int status;
            std::string url;
            iss >> key >> status >> url;
            currentRoute->setRedirectStatus(status);
            currentRoute->setRedirectUrl(url);
        }
        else if (line.find("root") == 0 && currentRoute) {
            currentRoute->setRootDirRoute(parseValue<std::string>(line));
        }
        else if (line.find("index") == 0 && currentRoute) {
            currentRoute->setIndexFile(parseValue<std::string>(line));
        }
        else if (line == "}") break;
    }
    if (currentRoute) {
        config.addRoute(currentRoute);
    }
}

static Config parseConfigFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open configuration file.");
    }
    
    Config config;
    std::string line;
    Route *currentRoute = nullptr;
    while (std::getline(file, line)) {
            line = trim(line);
            if (line.empty() || line[0] == '#') continue;
            
            if (line == "server {") continue;
            else if (line == "}") break;
            else if (line.find("listen") == 0) {
                config.setPort(parseValue<int>(line));
            } else if (line.find("server_name") == 0) {
                config.setName(parseValue<std::string>(line));
            } else if (line.find("root") == 0 && !currentRoute) {
                config.setRootDirConfig(parseValue<std::string>(line));
            } else if (line.find("client_max_body_size") == 0) {
                config.setMaxBodySize(parseValue<int>(line));
            } else if (line.find("index") == 0 && !currentRoute) {
                config.setDefaultFile(parseValue<std::string>(line));
            } else if (line.find("error_page") == 0) {
                std::istringstream iss(line);
                std::string key;
                int error_code;
                std::string error_page;
                iss >> key >> error_code >> error_page;
                config.setErrorPage(error_code, error_page);
            } else if (line.find("allow_methods") == 0) {
                std::vector<std::string> methods = parseMethods(line);
                if (currentRoute)
                    currentRoute->setAllowedMethods(methods);
                else
                    config.setAllowedMethods(methods);
            } else if (line.find("location") == 0)
                parseLocation(config, file, line);
        }
        if (currentRoute) {
            config.addRoute(currentRoute);
        }
        file.close();
        return config;
    }

private:
    static std::string trim(const std::string &str) {
        size_t first = str.find_first_not_of(" \t");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t");
        return str.substr(first, last - first + 1);
    }

    template<typename T>
    static T parseValue(const std::string &line) {
        std::istringstream iss(line);
        std::string key;
        T value;
        iss >> key >> value;
        return value;
    }

    static std::vector<std::string> parseMethods(const std::string &line) {
        std::istringstream iss(line);
        std::string key, method;
        std::vector<std::string> methods;
        iss >> key;
        while (iss >> method) {
            methods.push_back(method);
        }
        return methods;
    }

    static std::string parseLocationPath(const std::string &line) {
        size_t start = line.find("location") + 8;
        size_t end = line.find("{");
        return trim(line.substr(start, end - start));
    }
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
        return 1;
    }

    try {
        Config config = ConfigParser::parseConfigFile(argv[1]);
        config.printConfig();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
