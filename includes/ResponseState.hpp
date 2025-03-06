/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseState.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:05:39 by minakim           #+#    #+#             */
/*   Updated: 2025/03/06 16:36:13 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>

class Response;

class ResponseState
{
public:
	enum e_classes {
		_CAN_NOT_CLASSIFY = 0,
		INFORMATIONAL = 1,
		SUCCESSFUL,
		REDIRECTION,
		CLIENT_ERROR,
		SERVER_ERROR
	};
	enum e_statusFlags {
        STATUS_OK = 0,      		// default
        _STATUS_UNKNOWN = -1,		// in the range, but status not registed
        _STATUS_OUT_OF_RANGE = -2	// out of the range
    };
protected:
	int				_statusCode;
	e_classes		_statusClass;
	e_statusFlags	_statusFlag;
	bool			_responseExposed;

	static std::map<int, std::string> _scenarios;

public:
	ResponseState(int code);
	virtual ~ResponseState() {}

	// virtual std::string getStatus() const = 0;
	// virtual std::string getDefaultBody() const = 0;
	virtual bool				isWithinRange() const = 0;
	void						setStatusCode(int code);
	
	std::pair<int, std::string> getScenario();
	int							getStatusCode();
	e_classes					getStatusClasses();
	bool						isExposed();
	
	static std::map<int, std::string>&	getScenarios();
	static std::pair<int, std::string>	getScenario(int code);
	static void 						addNewScenario(int key, const std::string& value);

protected:
	void			_updateStatusFlag();
	void			_evaluateStatusCode();
	static void		_initDefaultScenario();
};

/* Static member initialization */
std::map<int, std::string> ResponseState::_scenarios;

/* Constructor */
ResponseState::ResponseState(int code) {
	if (_scenarios.empty()) {
		_initDefaultScenario();
	}
	setStatusCode(code);
	
}

/* Non-static Methods */

std::pair<int, std::string> ResponseState::getScenario() {
	if (_scenarios.find(_statusCode) != _scenarios.end()) {
		return (std::make_pair(_statusCode, _scenarios[_statusCode]));
	} else if (_statusCode >= 100 && _statusCode < 600) { 
		return std::make_pair(_STATUS_UNKNOWN, "Unknown status code");
	} else {
		return std::make_pair(_STATUS_OUT_OF_RANGE, "Not in the range");
	}
}

int ResponseState::getStatusCode() {
	return (_statusCode);
}

void	ResponseState::setStatusCode(int code) {
	_statusCode = code;
	_evaluateStatusCode();
}

ResponseState::e_classes	ResponseState::getStatusClasses () {
	return (_statusClass);
}

/* Static Methods */

std::map<int, std::string>& ResponseState::getScenarios() {
	return (_scenarios);
}

std::pair<int, std::string> ResponseState::getScenario(int code) {
	if (_scenarios.find(code) != _scenarios.end()) {
		return std::make_pair(code, _scenarios[code]);
	} else if (code >= 100 && code < 600) {
		return std::make_pair(_STATUS_UNKNOWN, "Unknown status code");
	} else {
		return std::make_pair(_STATUS_OUT_OF_RANGE, "Not in the range");
	}
}

void	ResponseState::addNewScenario(int key, const std::string& value) {
	if (_scenarios.find(key) == _scenarios.end()) {
		_scenarios[key] = value;
	}
}

bool	ResponseState::isExposed()
{
	return (_responseExposed);
}

/* Protected Method: Initialize Default Scenarios */
void	ResponseState::_updateStatusFlag() {
	_statusFlag = STATUS_OK;

	if (_statusCode >= 100 && _statusCode < 600) {
		if (_scenarios.find(_statusCode) == _scenarios.end()) {
			_statusFlag = _STATUS_UNKNOWN;
		}
	} else {
		_statusFlag = _STATUS_OUT_OF_RANGE;
	}
}

void	ResponseState::_evaluateStatusCode() {
    _responseExposed = true;

	if (_statusCode >= 100 && _statusCode < 600) {
		_statusClass = static_cast<e_classes>(_statusCode / 100);
        if (_statusClass == INFORMATIONAL) {
            _responseExposed = false;
        }
    } else {
        _statusClass = _CAN_NOT_CLASSIFY;
        _responseExposed = false;
    }
	_updateStatusFlag();
}

void	ResponseState::_initDefaultScenario() {
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
public:
	InformationalState(int code);
};

/* Successs */
class SuccessState : public ResponseState {
public:
	SuccessState();
	// std::string getStatus() const;
	// std::string getDefaultBody() const;
};

/* Error */
class ErrorState : public ResponseState {
public:
	ErrorState();

	// std::string getStatus() const;
	// std::string getDefaultBody() const;
};

/* Redirection */
class RedirectState : public ResponseState {
private:
	std::string	_location;

public:
	RedirectState(const std::string& url);

	// std::string getStatus() const;
	// std::string getDefaultBody() const;
};