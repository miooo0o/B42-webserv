/* 
#include "ResponseState.hpp"
#include "StatusManager.hpp"
#include "Request.hpp"
*/

#include "../../includes/ResponseState.hpp"
#include "../../includes/StatusManager.hpp"
#include "../../includes/Request.hpp"

////////////////////////////////////////////////////////////////////////////////

/* Static member initialization */
std::map<int, std::string> ResponseState::_scenarios;
bool	ResponseState::_isScenarioInitialized = false;

////////////////////////////////////////////////////////////////////////////////
/* Constructor */

ResponseState::ResponseState(Request& request, StatusManager& entries)
: _manager(entries), _request(request) {
}

////////////////////////////////////////////////////////////////////////////////

StatusManager&	ResponseState::getManager() {
	return (_manager);
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
// Informational
////////////////////////////////////////////////////////////////////////////////

InformationalState::InformationalState(Request& request, StatusManager& manager)
	: ResponseState(request, manager) {
}

std::string	InformationalState::getHandledBody(std::map<int, std::string>& serverScenarios) {
	(void)serverScenarios;
	return ("");
}

////////////////////////////////////////////////////////////////////////////////
// Success
////////////////////////////////////////////////////////////////////////////////

SuccessState::SuccessState(Request& request, StatusManager& entries)
	: ResponseState(request, entries) {
}

std::string	SuccessState::getHandledBody(std::map<int, std::string>& serverScenarios) {
	(void)serverScenarios;// KM_CHANGE
	return ("");
}
////////////////////////////////////////////////////////////////////////////////
//  Redirect
////////////////////////////////////////////////////////////////////////////////

RedirectState::RedirectState(Request& request, StatusManager& entries)
	: ResponseState(request, entries) {
}

std::string	RedirectState::getHandledBody(std::map<int, std::string>& serverScenarios) {
	(void)serverScenarios;// KM_CHANGE
	return ("");
}

////////////////////////////////////////////////////////////////////////////////
// Error
////////////////////////////////////////////////////////////////////////////////

ErrorState::ErrorState(Request& request, StatusManager& entries)
	: ResponseState(request, entries) {
}

std::string	ErrorState::getHandledBody(std::map<int, std::string>& serverScenarios) {
	(void)serverScenarios;// KM_CHANGE
	// int statusCode = _entries.getCode();
	// if (_entries.getMapRef() == Entry::_500_NOT_FOUND) {
	// 	/* force: return error string (not template) */

	// } else if (_entries.getMapRef() == Entry::REF_SERVER_CONFIG) {
	// 	/* return read file -> .html */

	// } else if (_entries.getMapRef() == Entry::REF_STATIC_MAP) {
	// 	/* return generate html */
	// }
	return ("");
}