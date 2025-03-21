/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseState.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:05:39 by minakim           #+#    #+#             */
/*   Updated: 2025/03/20 23:00:31 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSESTATE_HPP
# define RESPONSESTATE_HPP

#include <string>
#include <map>

class Response;
class Request;
class StatusManager;
class StatusEntry;

#include "StatusEntry.hpp"

class ResponseState {
protected:
	StatusManager&						_manager;
	Request&							_request;

	static std::map<int, std::string>	_scenarios;
	static bool							_isScenarioInitialized;

public:
	/* constructor */
	ResponseState(Request& reqeust, StatusManager& entries);

	// TODO: copy constructor, operator
	virtual ~ResponseState() {}
	
	/* virtual  */
	virtual std::string					getHandledBody(std::map<int, std::string>& serverScenarios) = 0;

	/* ststic methods: relate with map, `_scenarios` */
	static std::map<int, std::string>&	getScenarios();
	static void 						addNewScenario(int key, const std::string& value);

	StatusManager&						getManager();
protected:
	/* ststic methods */
	static void							_initDefaultScenario();
};

/**
 * Sub class -- ResponseState
 * 				1. InformationState
 * 				2. SuccessState
 * 				3. RedirectState 
 * 				4. ErrorState		(default)
 */

 /* informational */
class InformationalState : public ResponseState {
public:
	InformationalState(Request& request, StatusManager& manager);
	~InformationalState() {}

	std::string getHandledBody(std::map<int, std::string>& serverScenarios);

};

/* Successs */
class SuccessState : public ResponseState {
public:
	SuccessState(Request& request, StatusManager& entries);
	~SuccessState() {}

	std::string getHandledBody(std::map<int, std::string>& serverScenarios);
};

/* Redirection */
class RedirectState : public ResponseState {
public:
	RedirectState(Request& request, StatusManager& entries);
	~RedirectState() {}


	std::string getHandledBody(std::map<int, std::string>& serverScenarios);
};

/* Error */
class ErrorState : public ResponseState {
public:
	ErrorState(Request& request, StatusManager& entries);
	~ErrorState() {}

	std::string getHandledBody(std::map<int, std::string>& serverScenarios);
};

#endif