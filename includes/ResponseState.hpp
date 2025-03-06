/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseState.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:05:39 by minakim           #+#    #+#             */
/*   Updated: 2025/03/06 12:50:10 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>

#define STATUS_UNKNOWN	-1

class Response;

class ResponseState
{
public:
	typedef enum e_classes {
		_OUT_OF_RANGE = 0,
		INFORMATIONAL,
		SUCCESSFUL,
		REDIRECTION,
		CLIENT_ERROR,
		SERVER_ERROR
	};
protected:
	int			_status;
	e_classes	_statusClass;

	static std::map<int, std::string> _scenarios;
public:
	ResponseState(int code);
	virtual ~ResponseState() {}

	// virtual std::string getStatus() const = 0;
	// virtual std::string getDefaultBody() const = 0;

	void						setStatusCode(int code);
	
	std::pair<int, std::string> getScenario();
	int							getStatusCode();
	e_classes					getStatusClasses();
	
	static std::map<int, std::string>&	getScenarios();
	static std::pair<int, std::string>	getScenario(int code);
	static void 						addNewScenario(int key, const std::string& value);
	
	protected:
	void			_setStatusClass(int code);
	static void		_initDefaultScenario();
};

/* Static member initialization */
std::map<int, std::string> ResponseState::_scenarios;

/* Constructor */
ResponseState::ResponseState(int code) : _status(code) {
	if (_scenarios.empty()) {
		_initDefaultScenario();
	}
	_setStatusClass(_status);
}

/* Non-static Methods */

std::pair<int, std::string> ResponseState::getScenario() {
	if (_scenarios.find(_status) != _scenarios.end()) {
		return (std::make_pair(_status, _scenarios[_status]));
	}
	return (std::make_pair(STATUS_UNKNOWN, "Unknown Status Code"));
}

int ResponseState::getStatusCode() {
	if (_scenarios.find(_status) != _scenarios.end()) {
		return (_status);
	}
	return (STATUS_UNKNOWN);
}

void	ResponseState::setStatusCode(int code) {
	_status = code;
	_setStatusClass(_status);
}

void	ResponseState::_setStatusClass(int code) {
    if (code >= 100 && code < 200) {
        _statusClass = INFORMATIONAL;
    } else if (code >= 200 && code < 300) {
        _statusClass = SUCCESSFUL;
    } else if (code >= 300 && code < 400) {
        _statusClass = REDIRECTION;
    } else if (code >= 400 && code < 500) {
        _statusClass = CLIENT_ERROR;
    } else if (code >= 500 && code < 600) {
        _statusClass = SERVER_ERROR;
    } else {
        _statusClass = _OUT_OF_RANGE;
    }
}

ResponseState::e_classes	ResponseState::getStatusClasses () {
	return (_statusClass);	
}

/* Static Methods */

std::map<int, std::string>& ResponseState::getScenarios() {
	return _scenarios;
}

std::pair<int, std::string> ResponseState::getScenario(int code) {
	if (_scenarios.find(code) != _scenarios.end()) {
		return std::make_pair(code, _scenarios[code]);
	}
	return std::make_pair(STATUS_UNKNOWN, "Unknown Status Code");
}

void ResponseState::addNewScenario(int key, const std::string& value) {
	if (_scenarios.find(key) == _scenarios.end()) {
		_scenarios[key] = value;
	}
}

/* Protected Method: Initialize Default Scenarios */

void ResponseState::_initDefaultScenario() {
	_scenarios[100] = "Continue";
	_scenarios[200] = "OK";
	_scenarios[301] = "Moved Permanently";
	_scenarios[302] = "Found";
	_scenarios[303] = "See Other";
	_scenarios[307] = "Temporary Redirect";
	_scenarios[308] = "Permanent Redirect";
	_scenarios[400] = "Bad Request";
	_scenarios[401] = "Unauthorized";
	_scenarios[403] = "Forbidden";
	_scenarios[404] = "Not Found";
	_scenarios[500] = "Internal Server Error";
}

/* ************************************************************************** */

/* informational */
class InformationalState: public ResponseState {
	InformationalState(int code);
};

/* Successs */
class SuccessState : public ResponseState {
public:
	SuccessState(int code);
	SuccessState();

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