#pragma once

#include <string>
#include <map>

class Response;

class ResponseState
{
protected:
	int                        			_status;
	static std::map<int, std::string>	_defaultScenario;
public:
	ResponseState(int code) : _status(code) { _initDefaultScenario(); }
	virtual ~ResponseState() {}
	virtual std::string getStatus() const = 0;
	virtual std::string getDefaultBody() const = 0;
	static void addNewScenario(int key, const std::string& value) {
		if (_defaultScenario.find(key) == _defaultScenario.end()) {
			_defaultScenario[key] = value;
		}
	}

protected:
	static void _initDefaultScenario() {
		_defaultScenario[200] = "OK";
		_defaultScenario[301] = "Moved Permanently";
		_defaultScenario[302] = "Found";
		_defaultScenario[303] = "See Other";
		_defaultScenario[307] = "Temporary Redirect";
		_defaultScenario[308] = "Permanent Redirect";
		_defaultScenario[400] = "Bad Request";
		_defaultScenario[401] = "Unauthorized";
		_defaultScenario[403] = "Forbidden";
		_defaultScenario[404] = "Not Found";
		_defaultScenario[500] = "Internal Server Error";
	}
};

/* Successs */
class SuccessState : public ResponseState {
public:
	std::string getStatus() const;
	std::string getDefaultBody() const;
};

/* Error */
class ErrorState : public ResponseState {
public:
	ErrorState(int code);

	std::string getStatus() const;
	std::string getDefaultBody() const;
};

/* Redirection */
class RedirectState : public ResponseState {
private:
	std::string _location;

public:
	RedirectState(const std::string& url);

	std::string getStatus() const;
	std::string getDefaultBody() const;
};