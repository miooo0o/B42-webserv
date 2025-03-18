#include "ResponseState.hpp"
#include "Reqeust.hpp"
#include "Entries.hpp"

////////////////////////////////////////////////////////////////////////////////

/* Static member initialization */
std::map<int, std::string> ResponseState::_scenarios;
bool	ResponseState::_isScenarioInitialized = false;

////////////////////////////////////////////////////////////////////////////////
/* Constructor */

ResponseState::ResponseState(Request& reqeust) 
: _request(reqeust), _entries(_request) {
}

////////////////////////////////////////////////////////////////////////////////

bool	ResponseState::isEntryUnprocessed() {
	if (_entries.getQueueStatus() == Entries::QUEUE_FULL) {
		return (true);
	}
	return (false);
}

bool	ResponseState::hasValidatedEntry() {
	if (_entries.getQueueStatus() > Entries::QUEUE_EMPTY) {
		if (_entries.getEntry().isReady()) {
			return (true);
		}
	}
	return (false);
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


/*
	int statusCode = _entries.getEntry().getCode();
	if (serverScenarios.find(statusCode) != serverScenarios.end()) {
		return ("");			// read file
	} else if (_scenarios.find(statusCode) != _scenarios.end()) {
		return ("");			// html generate
	} else {
		_entries.replace(500);	// error_code;
	}
*/

////////////////////////////////////////////////////////////////////////////////
// Informational
////////////////////////////////////////////////////////////////////////////////

InformationalState::InformationalState(Request& request) : ResponseState(request) {
}

std::string	InformationalState::getHandledBody(std::map<int, std::string>& serverScenarios) {
	return ("");
}

////////////////////////////////////////////////////////////////////////////////
// Success
////////////////////////////////////////////////////////////////////////////////

SuccessState::SuccessState(Request& request) : ResponseState(request) {
}

std::string	SuccessState::getHandledBody(std::map<int, std::string>& serverScenarios) {
	
	int statusCode = _entries.getCode();

}
////////////////////////////////////////////////////////////////////////////////
//  Redirect
////////////////////////////////////////////////////////////////////////////////

RedirectState::RedirectState(Request& request) : ResponseState(request) {
}

std::string	RedirectState::getHandledBody(std::map<int, std::string>& serverScenarios) {
	
	// TODO: check: redirection logic

}

////////////////////////////////////////////////////////////////////////////////
// Error
////////////////////////////////////////////////////////////////////////////////

ErrorState::ErrorState(Request& request) : ResponseState(request) {
}

std::string	ErrorState::getHandledBody(std::map<int, std::string>& serverScenarios) {
	int statusCode = _entries.getCode();
	if (_entries.getMapRef() == Entry::_500_NOT_FOUND) {
		/* force: return error string (not template) */

	} else if (_entries.getMapRef() == Entry::REF_SERVER_CONFIG) {
		/* return read file -> .html */

	} else if (_entries.getMapRef() == Entry::REF_STATIC_MAP) {
		/* return generate html */
	}
}