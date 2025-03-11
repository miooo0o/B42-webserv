/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseState.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:05:39 by minakim           #+#    #+#             */
/*   Updated: 2025/03/10 20:29:22 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>

#include "HtmlHandler.hpp"

class Response;

class ResponseState {
public:
	/**
	 * @brief Status validation flags for the ResponseState class.
	 */
	enum e_flag {
		FLAG_PENDING         	= 0,	/* Default state, pending validation */
	    FLAG_VALIDATED       	= 1,	/* Status code is valid and registered */
	    _FLAG_UNKNOWN        	= -1,	/* Code is within valid range but not registered */
	    _FLAG_OUT_OF_RANGE   	= -2,	/* Code is outside the HTTP status code range (100-599) */
	    _FLAG_NOT_CODE_STATUS	= -3	/* Code is not a valid HTTP status (e.g., negative or too large) */
	};
	/** 
 	* @brief Represents the classification of HTTP status codes into categories.
 	*/
	enum e_class {
    	_NOT_CLASSIFY			= 0,	/* Default unclassified state */
    	INFORMATIONAL			= 1,	/* 1xx: Informational responses */
    	SUCCESSFUL				= 2,	/* 2xx: Successful responses */
    	REDIRECTION				= 3,	/* 3xx: Redirection messages */
    	CLIENT_ERROR			= 4,	/* 4xx: Client error responses */
    	SERVER_ERROR			= 5 	/* 5xx: Server error responses */
	};
	struct Status {
		int		_code;
		e_flag	_flag;
		e_class	_class;
		bool	_responseExposed;

		Status::Status() : _code(500), _flag(FLAG_PENDING), _class(_NOT_CLASSIFY) {};
		bool	Status::isValidated() { return (_flag == FLAG_VALIDATED && _class != _NOT_CLASSIFY); } 
	};

protected:
	Status			_status;

	static std::map<int, std::string>	_scenarios;
	static bool							_isScenarioInitialized;

public:
	ResponseState(int code);
	virtual ~ResponseState() {}
	// (need?) copy constructor
	
	virtual std::string getHandledBody() const = 0;

	bool						setStatusCode(int code);
	bool						updateSubStatusCode(int code);
	bool						isValidated();

	/* getter */
	int							getStatusCode();
	e_flag						getStatusFlag();
	e_class						getStatusClass();

	/* is methods */
	bool						isExposed();
	bool						isStatusCodeInClassRange(int code);
	bool						isStatusCode(int code);

	/* ststic methods */
	static std::map<int, std::string>&	getScenarios();
	static void 						addNewScenario(int key, const std::string& value);
	
protected:
	void			_evaluateStatus();
	e_flag			_validateStatusCode(int code);
	e_class			_classifyStatusCode(int code);

	/* ststic methods */
	static void		_initDefaultScenario();
};

////////////////////////////////////////////////////////////////////////////////

/* Static member initialization */
std::map<int, std::string> ResponseState::_scenarios;
bool	ResponseState::_isScenarioInitialized = false;

////////////////////////////////////////////////////////////////////////////////
/* Constructor */

ResponseState::ResponseState(int code) : _status(), _htmlHandler(_status) {
	_initDefaultScenario();
	setStatusCode(code);
}

////////////////////////////////////////////////////////////////////////////////
// NOTE to Team:
//
// The following two methods (setStatusCode, setSubStatusCode) are expected to be 
// the most commonly used. If you encounter any inconvenience while using them 
// (e.g., needing to call multiple methods or facing unexpected behavior), 
// please feel free to reach out to @minakim.
//
// Example:
// if (!state->setSubStatusCode(204) || !state->setStatusCode(204)) {
//     state = new ErrorState(500); // Automatically handle invalid status
// }
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Sets a new status code and evaluates its validity.
 * 
 * This method directly sets a new HTTP status code and automatically evaluates
 * the status. It is used when completely resetting or changing the response state,
 * such as during initialization or when transitioning to a different status class.
 * 
 * @param code The new HTTP status code to set (expected range: 100 to 599).
 * @return `bool` True if the status code is valid and properly classified.
 *              False if the status code is invalid or not registered.
 * 
 * Example usage:
 * @code
 * ResponseState* state = new SuccessState(200);
 * if (!state->setStatusCode(500)) {
 *     state = new ErrorState(500); // Automatically handle invalid status
 * }
 * @endcode
 */
bool	ResponseState::setStatusCode(int code) {
	_status._code = code;
	_evaluateStatus();
	return (isValidated());
}

/**
 * @brief Sets a new status code within the current status class.
 * 
 * Updates the status code only if it is within the same e_class range.
 * Automatically evaluates the status and allows simplified error handling
 * using the isValidated() method.
 * 
 * @param code The HTTP status code to update (must remain in the current class range).
 * @return `bool` True if the status code is valid and updated successfully.
 *              False if the status code is out of the expected class range.
 * 
 * Example usage:
 * @code
 * ResponseState* state = new SuccessState(200);
 * if (!state->setSubStatusCode(204)) {
 *     state = new ErrorState(500); // Automatically handle invalid status
 * }
 * @endcode
 */
bool	ResponseState::updateSubStatusCode(int code) {
    if (isStatusCodeInClassRange(code)) {
        setStatusCode(code);
        return (isValidated());
    }
    return (false);
}

bool	ResponseState::isValidated() {
	return (_status.isValidated());
}

////////////////////////////////////////////////////////////////////////////////
/* getter */

int ResponseState::getStatusCode() {
	return (_status._code);
}

ResponseState::e_flag	ResponseState::getStatusFlag () {
	return (_status._flag);
}

ResponseState::e_class	ResponseState::getStatusClass () {
	return (_status._class);
}

////////////////////////////////////////////////////////////////////////////////
/* is methods */

bool	ResponseState::isExposed() {
	return (_status._responseExposed);
}

bool	ResponseState::isStatusCodeInClassRange(int code) {
	return (code >= _status._class * 100 && code < _status._class * 100 + 100);
}

bool	ResponseState::isStatusCode(int code) {
	return (code >= 100 && code < 600);
}


////////////////////////////////////////////////////////////////////////////////
/* Protected Method, only called when constructor structed */

void	ResponseState::_evaluateStatus() {
    _status._flag = _validateStatusCode(_status._code);
    if (_status._flag != FLAG_VALIDATED) {
        return ;
    }
    _status._class = _classifyStatusCode(_status._code);
    _status._responseExposed = _status._class != INFORMATIONAL;
}


ResponseState::e_flag ResponseState::_validateStatusCode(int code) {
    if (!isStatusCode(code)) {
        return (_FLAG_NOT_CODE_STATUS);
    }
    if (_scenarios.find(code) == _scenarios.end()) {
        return (_FLAG_UNKNOWN);
    }
    return (FLAG_VALIDATED);
}

ResponseState::e_class ResponseState::_classifyStatusCode(int code) {
    return (static_cast<e_class>(code / 100));
}


////////////////////////////////////////////////////////////////////////////////
/* Static Methods */

std::map<int, std::string>& ResponseState::getScenarios() {
	_initDefaultScenario();
    return (_scenarios);
}

void	ResponseState::addNewScenario(int key, const std::string& value) {
	if (_scenarios.find(key) == _scenarios.end()) {
		_scenarios[key] = value;
	}
}

void	ResponseState::_initDefaultScenario() {
	if (_isScenarioInitialized) {
		return ;
	}
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
	_isScenarioInitialized = true;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * Sub class -- ResponseState
 * 				1. InformationState	:
 * 				2. SuccessState		:
 * 				3. RedirectState	: 
 * 				4. ErrorState		:
 */

 /* informational */
class InformationalState: public ResponseState {
public:
	InformationalState(int code);
};

/* Successs */
class SuccessState : public ResponseState {
public:
	SuccessState(int code);
	// std::string getStatus() const;
	// std::string getHandledBody() const;
};

/* Error */
class ErrorState : public ResponseState {
public:
	ErrorState(int code);
	// std::string getStatus() const;
	// std::string getHandledBody() const;
};

/* Redirection */
class RedirectState : public ResponseState {
public:
	// RedirectState(const std::string& url /* , location */);
	RedirectState(int code);

	// std::string getStatus() const;
	// std::string getHandledBody() const;
};