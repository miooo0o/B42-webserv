#include "Route.hpp"
#include <iostream>

// Constructor
Route::Route() : autoindex(false), redirect_status(0), autoindex_set(false) {}

// Copy Constructor
Route::Route(const Route &other)
    : path(other.path), allowed_methods(other.allowed_methods),
      root_dir(other.root_dir), index_file(other.index_file),
      autoindex(other.autoindex), redirect_status(other.redirect_status),
      redirect_url(other.redirect_url), autoindex_set(other.autoindex_set) {}

// Assignment Operator
Route Route::operator=(const Route &another) {
    if (this != &another) {
        path = another.path;
        allowed_methods = another.allowed_methods;
        root_dir = another.root_dir;
        index_file = another.index_file;
        autoindex = another.autoindex;
        redirect_status = another.redirect_status;
        redirect_url = another.redirect_url;
        autoindex_set = another.autoindex_set;
    }
    return *this;
}

// Destructor
Route::~Route() {}

// Setters
void Route::setPath(const std::string &path) { this->path = path; }
void Route::setAllowedMethods(const std::vector<std::string> &allowed_methods) { this->allowed_methods = allowed_methods; }
void Route::setRootDirRoute(const std::string &root_dir) { this->root_dir = root_dir; }
void Route::setIndexFile(const std::string &index_file) { this->index_file = index_file; }
void Route::setAutoindex(const std::string& autoindex)
{
    autoindex_set = true;
    if (autoindex.find("on") == 0)
    {
        this->autoindex = true;
    }
    else if(autoindex.find("off") == 0)
    {
        this->autoindex = false;
    }
    else
        autoindex_set = false;
}
void Route::setRedirectStatus(int redirect_status) { this->redirect_status = redirect_status; }
void Route::setRedirectUrl(const std::string &redirect_url) { this->redirect_url = redirect_url; }

// Check if route contains the target path
bool Route::containsTarget(const std::string &target) const {
    return target.find(path) == 0;
}

// Check if method is allowed
bool Route::allowsMethod(const std::string &method) const {
    for (size_t i = 0; i < allowed_methods.size(); i++) {
        if (allowed_methods[i] == method)
            return true;
    }
    return false;
}

// Route validation check
int Route::checkRoute(Route *route) {
    if (route->path.empty()) return 0;
    return 1;
}

// Cleanup
void Route::cleanRoute(Route *route) {
    route->allowed_methods.clear();
}

// Print allowed methods
std::ostream& Route::printPermittedMethods(std::ostream &os) {
    for (size_t i = 0; i < allowed_methods.size(); i++) {
        os << allowed_methods[i];
		if (i < allowed_methods.size() - 1)
       		os << ", ";
		else
			os << std::endl;
    }
	return os;
}

// Print route details
std::ostream& Route::printRoute(std::ostream& os) {
    os << "Path: " << path << "\n"
              << "Root Dir: " << root_dir << "\n"
              << "Index File: " << index_file << "\n"
              << "Autoindex: " << (autoindex ? "On" : "Off") << "\n"
              << "Redirect Status: " << redirect_status << "\n"
              << "Redirect URL: " << redirect_url << "\n";
	printPermittedMethods(os);
	return os;
}

void Route::printRoute() {

    std::cout << "Path: " << path << "\n"
              << "Root Dir: " << root_dir << "\n"
              << "Index File: " << index_file << "\n"
              << "Autoindex: " << (autoindex ? "On" : "Off") << "\n"
              << "Redirect Status: " << redirect_status << "\n"
              << "Redirect URL: " << redirect_url << "\n";

}