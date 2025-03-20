/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseState.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:05:39 by minakim           #+#    #+#             */
/*   Updated: 2025/03/20 20:58:02 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>

class Response;
class Request;
class Entries;
class Entry;

#include "Entries.hpp"

class ResponseState {
protected:
	Entries								_entries;
	Request&							_request;

	static std::map<int, std::string>	_scenarios;
	static bool							_isScenarioInitialized;

public:
	/* constructor */
	ResponseState(Request& reqeust);
	// TODO: copy constructor, operator
	virtual ~ResponseState() {}
	
	/* virtual  */
	virtual std::string getHandledBody(std::map<int, std::string>& serverScenarios) = 0;
	virtual void		updateStatus(const Entry& entry);
	/* ststic methods: relate with map, `_scenarios` */
	static std::map<int, std::string>&	getScenarios();
	static void 						addNewScenario(int key, const std::string& value);
protected:
	/* ststic methods */
	static void			_initDefaultScenario();
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
	InformationalState(Request& request);
	~InformationalState();

	std::string getHandledBody(std::map<int, std::string>& serverScenarios);

};

/* Successs */
class SuccessState : public ResponseState {
public:
	SuccessState(Request& request);
	~SuccessState();

	std::string getHandledBody(std::map<int, std::string>& serverScenarios);
};

/* Redirection */
class RedirectState : public ResponseState {
public:
	RedirectState(Request& request);
	~RedirectState();


	std::string getHandledBody(std::map<int, std::string>& serverScenarios);
};

/* Error */
class ErrorState : public ResponseState {
public:
	ErrorState(Request& request);
	~ErrorState();

	std::string getHandledBody(std::map<int, std::string>& serverScenarios);
};
