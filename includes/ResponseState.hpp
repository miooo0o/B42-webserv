/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseState.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:05:39 by minakim           #+#    #+#             */
/*   Updated: 2025/03/06 18:16:39 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>

class Response;

class ResponseState
{
public:
	enum e_class {
		INFORMATIONAL	= 1,
		SUCCESSFUL,
		REDIRECTION,
		CLIENT_ERROR,
		SERVER_ERROR
	};
	enum e_flag {
        STATUS_OK					= 0,	// default
        _STATUS_UNKNOWN 			= -1,	// in the range, but status not registed
        _STATUS_OUT_OF_RANGE 		= -2,	// out of the range
    };
protected:
	struct Status {
    	int		_code;
		e_flag	_flag;
		e_class	_class;
	};
protected:
	Status			_status;
	bool			_responseExposed;

	static std::map<int, std::string> _scenarios;

public:
	ResponseState(int code);
	virtual ~ResponseState() {}

	// virtual std::string getStatus() const = 0;
	// virtual std::string getDefaultBody() const = 0;

	virtual bool				isWithinRange() const = 0;
	void						setStatusCode(int code);
	
	int							getStatusCode();
	e_class						getStatusClass();
	e_flag						getStatusFlag();
	bool						isExposed();
	
	static std::map<int, std::string>&	getScenarios();
	static void 						addNewScenario(int key, const std::string& value);

protected:
	void			_evaluateStatus();
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

int ResponseState::getStatusCode() {
	return (_status._code);
}

void	ResponseState::setStatusCode(int code) {
	_status._code = code;
	_evaluateStatus();
}

ResponseState::e_class	ResponseState::getStatusClass () {
	return (_status._class);
}

/* Static Methods */

std::map<int, std::string>& ResponseState::getScenarios() {
	return (_scenarios);
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

void	ResponseState::_evaluateStatus() {

	bool	isInRange = _status._code >= 100 && _status._code < 600;
	if (!isInRange) {
		_status._flag = _STATUS_OUT_OF_RANGE;
		return ;
	}
	if (_scenarios.find(_status._code) == _scenarios.end()) {
		_status._flag = _STATUS_UNKNOWN;
		return ;
	}
	_status._flag = STATUS_OK;
	_status._class = static_cast<e_class>(_status._code / 100);
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
	
	bool				isWithinRange();
};

/* Successs */
class SuccessState : public ResponseState {
public:
	SuccessState();

	bool				isWithinRange();
	// std::string getStatus() const;
	// std::string getDefaultBody() const;
};

/* Error */
class ErrorState : public ResponseState {
public:
	ErrorState();

	bool				isWithinRange();
	// std::string getStatus() const;
	// std::string getDefaultBody() const;
};

/* Redirection */
class RedirectState : public ResponseState {
private:
	std::string	_location;

public:
	RedirectState(const std::string& url);

	bool				isWithinRange();
	// std::string getStatus() const;
	// std::string getDefaultBody() const;
};