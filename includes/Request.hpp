
#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <map>

class Request {
public:
	Request(int code) : _code(code) {
		_serverScenarios[100] = "Continue";
		_serverScenarios[200] = "OK";
		_serverScenarios[400] = "Bad Request";
		_serverScenarios[404] = "Not Found";
		_serverScenarios[500] = "Internal Server Error";
	}
	~Request() {}
	int	getCode() const { return _code; }
	void setCode(int code) { _code = code; }
	std::map<int, std::string>& getServerMap() { return _serverScenarios; }

private:
	int                         _code;
	std::map<int, std::string>  _serverScenarios;
};

#endif //REQUEST_HPP
