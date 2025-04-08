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
#include <sstream>

class Response;
class Request;
class StatusManager;
class StatusEntry;

#include "StatusEntry.hpp"

class ResponseState {
protected:
	Request*							_request;
	StatusManager&						_manager;
	StatusEntry*						_currentEntry;

	static std::map<int, std::string>	_scenarios;
	static bool							_isScenarioInitialized;

public:
	/* constructor */
	ResponseState(Request* reqeust, StatusManager& entries);

	// TODO: copy constructor, operator
	virtual ~ResponseState() {}
	
	/* virtual  */
	virtual std::string					getHandledBody(std::map<int, std::string>& serverScenarios) = 0;

	/* related with map, `_scenarios` */
	static std::map<int, std::string>&	getScenarios();
	static void 						addNewScenario(int key, const std::string& value);

	StatusManager&						getManager();

	void								setCurrentEntry(StatusEntry* entry);
	StatusEntry*						getCurrentEntry() const;

protected:
	/* */

	/* HTML generate */
	std::string			_generateHtml(const std::string& title,
							 const std::string& message,
							 const std::string& details = "",
							 const std::string& additionalHtml = "");

	std::string 		_generateDoctype() const;
	std::string 		_generateHead(const std::string& title) const;
	std::string 		_generateStyles() const;
	std::string 		_generateBodyStart() const;
	std::string 		_generateBodyEnd() const;

	virtual std::string _generateAdditionalStyles() const;
	virtual std::string _generateContainer(const std::string& title,
                                            const std::string& message,
                                            const std::string& details,
                                            const std::string& additionalHtml) const;
	virtual std::string _generateTitle(const std::string& title) const;
	virtual std::string _generateMessage(const std::string& message) const;
	virtual std::string _generateDetails(const std::string& details) const;

	/* related with map, `_scenarios` */
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
	InformationalState(Request* request, StatusManager& manager);
	~InformationalState() {}

	std::string getHandledBody(std::map<int, std::string>& serverScenarios);

};

/* Successs */
class SuccessState : public ResponseState {
public:
	SuccessState(Request* request, StatusManager& entries);
	~SuccessState() {}

	std::string getHandledBody(std::map<int, std::string>& serverScenarios);
};

/* Redirection */
class RedirectState : public ResponseState {
public:
	RedirectState(Request* request, StatusManager& entries);
	~RedirectState() {}


	std::string getHandledBody(std::map<int, std::string>& serverScenarios);
};

/* Error */
class ErrorState : public ResponseState {
public:
	ErrorState(Request* request, StatusManager& entries);
	~ErrorState() {}

	std::string getHandledBody(std::map<int, std::string>& serverScenarios);
private:
	bool	_call_static;
};

#endif