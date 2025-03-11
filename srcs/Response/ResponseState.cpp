#include "ResponseState.hpp"
////////////////////////////////////////////////////////////////////////////////

/* Static member initialization */
std::map<int, std::string> ResponseState::_scenarios;
bool	ResponseState::_isScenarioInitialized = false;

////////////////////////////////////////////////////////////////////////////////
/* Constructor */

ResponseState::ResponseState(Request* reqeust) : _request(reqeust), _status() {
}

ResponseState::ResponseState(int code) : _request(NULL), _status() {
	_initDefaultScenario();
	setStatusCode(code);
}

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
