/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseState.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:05:39 by minakim           #+#    #+#             */
/*   Updated: 2025/03/11 14:46:10 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>
#include "ContentHandler.hpp"

class Response;
class Request;
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
	Status								_status;
	Request*							_request;
	ContentHandler						_contentHandle;

	static std::map<int, std::string>	_scenarios;
	static bool							_isScenarioInitialized;

public:
	ResponseState(int code);
	ResponseState(Request* reqeust);

	virtual ~ResponseState() {}
	// (need?) copy constructor
	
	// virtual std::string getHandledBody() const = 0;

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
	SuccessState(Request& request);
	// std::string getStatus() const;
	// std::string getHandledBody() const;
};

/* Error */
class ErrorState : public ResponseState {
public:
	ErrorState(int code);
	ErrorState(Request& request);
	// std::string getStatus() const;
	// std::string getHandledBody() const;
};

/* Redirection */
class RedirectState : public ResponseState {
public:
	// RedirectState(const std::string& url /* , location */);
	RedirectState(int code);
	RedirectState(Request& request);


	// std::string getStatus() const;
	// std::string getHandledBody() const;
};