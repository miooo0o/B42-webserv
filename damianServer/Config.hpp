// Config.hpp
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include <map>
#include "Route.hpp"
#include "../includes/core_def.hpp"

class Config {
private:
    int server_port;
    std::string server_name;
    std::string root_dir;
    int max_body_size;
    std::string default_file;

    std::map<int, std::string> error_pages;
    std::vector<std::string> allowed_methods;
    std::vector<Route *> routes;

public:
    Config();
    Config(const Config &other);
    Config operator=(const Config &another);
    ~Config();

    // Setters
    void setPort(int port);
    void setName(std::string name);
    void setRootDirConfig(std::string root_dir);
    void setMaxBodySize(int max_body_size);
    void setDefaultFile(std::string default_file);
    void setErrorPage(int error_status, std::string error_page);
    void setAllowedMethods(const std::vector<std::string> &allowed_methods);
    void addRoute(Route *route);

    // Getters
    int getPort() const;
    std::string getName() const;
    std::string getRootDirConfig() const;
    int getMaxBodySize() const;
    std::string getDefaultFile() const;
    const std::map<int, std::string> &getErrorPages() const;
    const std::vector<std::string> &getAllowedMethods() const { return this->allowed_methods; };
    const std::vector<Route *> &getRoutes() const { return this->routes; };

    Route *getRouteForTarget(const std::string &target);

	// Printing
	std::ostream&	outputPermittedMethods(std::ostream& os);
	std::ostream&	outputRoutes(std::ostream& os);

    // Utils
    int initialisedCheck();
    void printConfig() const;
    void freeConfig();
};

std::ostream& operator<<(std::ostream& os, Config* config);
#endif // CONFIG_HPP