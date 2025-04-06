#include "ResponseState.hpp"
#include "StatusManager.hpp"

/**
 * Static member initialization
 */
std::map<int, std::string> ResponseState::_scenarios;
bool	ResponseState::_isScenarioInitialized = false;

////////////////////////////////////////////////////////////////////////////////
// ResponseState
////////////////////////////////////////////////////////////////////////////////

ResponseState::ResponseState(Request& request, StatusManager& entries)
: _request(request), _manager(entries), _currentEntry(NULL) {}


////////////////////////////////////////////////////////////////////////////////
// Informational
////////////////////////////////////////////////////////////////////////////////

InformationalState::InformationalState(Request& request, StatusManager& manager)
	: ResponseState(request, manager) {
	if (_currentEntry == NULL)
		throw std::logic_error("No current entry");
}

std::string	InformationalState::getHandledBody(std::map<int, std::string>& serverScenarios) {
	return ("");
}

////////////////////////////////////////////////////////////////////////////////
// Success
////////////////////////////////////////////////////////////////////////////////

SuccessState::SuccessState(Request& request, StatusManager& entries)
	: ResponseState(request, entries) {
	if (_currentEntry == NULL)
		throw std::logic_error("No current entry");
}

std::string	SuccessState::getHandledBody(std::map<int, std::string>& serverScenarios) {
	if (_currentEntry->getCode() == 204)
		return ("");
	return ("");
}

////////////////////////////////////////////////////////////////////////////////
// Redirect
////////////////////////////////////////////////////////////////////////////////

RedirectState::RedirectState(Request& request, StatusManager& entries)
	: ResponseState(request, entries) {
	if (_currentEntry == NULL)
		throw std::logic_error("No current entry");
}

std::string	RedirectState::getHandledBody(std::map<int, std::string>& serverScenarios) {
	return ("");
}

////////////////////////////////////////////////////////////////////////////////
// Error
////////////////////////////////////////////////////////////////////////////////

ErrorState::ErrorState(Request& request, StatusManager& entries)
	: ResponseState(request, entries) {
	if (_currentEntry == NULL)
		throw std::logic_error("No current entry");
}


std::string	ErrorState::getHandledBody(std::map<int, std::string>& serverScenarios) {
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


////////////////////////////////////////////////////////////////////////////////
// ResponseState (base)
////////////////////////////////////////////////////////////////////////////////

/* getter */
StatusManager&	ResponseState::getManager() {
	return (_manager);
}

StatusEntry*	ResponseState::getCurrentEntry() const {
	return (_currentEntry);
}

/* setter */
void			ResponseState::setCurrentEntry(StatusEntry* entry) {
	_currentEntry = entry;
}

////////////////////////////////////////////////////////////////////////////////
// Static methods -- Scenarios, TODO: can be change depend on Request
////////////////////////////////////////////////////////////////////////////////

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
// ResponseState, HTML generate methods
////////////////////////////////////////////////////////////////////////////////

/* TODO: not decide template design yet. can be change.
 * @note: css, html
 */

std::string ResponseState::_generateHtml(const std::string& title,
							 const std::string& message,
							 const std::string& details,
							 const std::string& additionalHtml) {
	std::stringstream ss;

	ss << _generateDoctype();
	ss << _generateHead(title);
	ss << _generateBodyStart();
	ss << _generateContainer(title, message, details, additionalHtml);
	ss << _generateBodyEnd();
	return (ss.str());
}

std::string ResponseState::_generateDoctype() const {
    return ("<!DOCTYPE html>\n");
}

std::string ResponseState::_generateHead(const std::string& title) const {
    std::stringstream	ss;
    ss << "<html>\n<head>\n"
       << "<title>" << title << "</title>\n"
       << _generateStyles()
       << "</head>\n";
    return (ss.str());
}

std::string ResponseState::_generateStyles() const {
    std::stringstream	ss;
    ss << "<style>\n"
       << "body { font-family: Arial, sans-serif; margin: 40px; line-height: 1.6; }\n"
       << "h1 { color: #333; }\n"
       << ".container { border: 1px solid #ddd; padding: 20px; border-radius: 5px; }\n"
       << _generateAdditionalStyles()
       << "</style>\n";
    return (ss.str());
}

std::string ResponseState::_generateAdditionalStyles() const {
    return ("");
}

std::string ResponseState::_generateBodyStart() const {
    return ("<body>\n");
}

std::string ResponseState::_generateContainer(const std::string& title,
                              const std::string& message,
                              const std::string& details,
                              const std::string& additionalHtml) const {
    std::stringstream	ss;

    ss << "<div class='container'>\n";
    ss << _generateTitle(title);
    ss << _generateMessage(message);
    if (!details.empty()) {
        ss << _generateDetails(details);
    }
    if (!additionalHtml.empty()) {
        ss << additionalHtml << "\n";
    }
    ss << "</div>\n";
    return (ss.str());
}

std::string ResponseState::_generateTitle(const std::string& title) const {
    return ("<h1>" + title + "</h1>\n");
}

std::string ResponseState::_generateMessage(const std::string& message) const {
    return ("<p>" + message + "</p>\n");
}

std::string ResponseState::_generateDetails(const std::string& details) const {
    return ("<div class='details'><p>" + details + "</p></div>\n");
}

std::string ResponseState::_generateBodyEnd() const {
    return ("</body>\n</html>");
}