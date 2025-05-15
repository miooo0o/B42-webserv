#include "StatusQueue.hpp"
#include "StatusManager.hpp"
#include "Request.hpp"
#include "ResponseState.hpp"
#include "StatusEntry.hpp"

////////////////////////////////////////////////////////////////////////////////
// StatusManager
////////////////////////////////////////////////////////////////////////////////

StatusManager::StatusManager(const Request* request)
	: _request(request) {
	_initEntry();
}

StatusManager::StatusManager(int statusCode)
	: _request(NULL) {
	_initEntry(statusCode);
}

StatusManager::StatusManager(const StatusManager& other) {
	_statusQueue = other._statusQueue;
}

StatusManager::~StatusManager() {}

////////////////////////////////////////////////////////////////////////////////

void	StatusManager::_notifyObservers() {
	if (_observers.empty())
		return ;
	for (std::vector<EntryObserver*>::iterator it = _observers.begin();
		 it != _observers.end(); ++it) {
		(*it)->_onEntryChanged();
	}
}

void	StatusManager::addObserver(EntryObserver* observer) {
	_observers.push_back(observer);
}

void	StatusManager::removeObserver(EntryObserver* observer) {
	for (std::vector<EntryObserver*>::iterator it = _observers.begin(); it != _observers.end(); ++it) {
		if (*it == observer) {
			_observers.erase(it);
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////


void    StatusManager::_initEntry() {
	if (!_statusQueue.empty()) {
		throw std::logic_error("Entries queue is not empty during initialization.");
	}
	int requestStatusCode =  /* get error code from _request */ ;
	_statusQueue.push(StatusEntry(requestStatusCode));
}

void    StatusManager::_initEntry(int statusCode) {
	if (!_statusQueue.empty()) {
		throw std::logic_error("Entries queue is not empty during initialization.");
	}
	_statusQueue.push(StatusEntry(statusCode));
}

void	StatusManager::push(StatusEntry status) {
	_statusQueue.push(status);
	_notifyObservers();
}

void	StatusManager::pop() {
	_statusQueue.pop();
}

////////////////////////////////////////////////////////////////////////////////

void	StatusManager::archiveAll() {
	while (!_statusQueue.empty()) {
		_statusQueue.archiveProcessed();
	}
}

void	StatusManager::reuseEntry(int newCode) {
	_statusQueue.front().setCode(newCode);
	_statusQueue.front().setRange(StatusEntry::RANGE_PENDING);
	_statusQueue.front().setClass(StatusEntry::_NOT_CLASSIFY);
	_statusQueue.front().setExposed(false);
	_statusQueue.front().setErrorSource(StatusEntry::SRC_ERROR_CLASS);
	_statusQueue.front().setFlow(StatusEntry::FLOW_PENDING_REUSE);
}

////////////////////////////////////////////////////////////////////////////////

bool StatusManager::ready() {
	return (!_statusQueue.empty());
}


bool	StatusManager::eval(const std::map<int, std::string>* serverScenarios) {
	if (!ready()) return (false);
	_validateCodeRange();
	_findCodeReference(serverScenarios);
	StatusEntry&	entry = _statusQueue.front();
	if (_isReadyToClassify(entry)) {
		_autoSetClasses(entry);
		entry.setExposed(_statusQueue.front().getClass() != StatusEntry::INFORMATIONAL);
		entry.setFlow(StatusEntry::FLOW_READY);
		return (true);
	}
	entry.setError(StatusEntry::ERROR_EVALUATION);
	return (false);
}

bool	StatusManager::_isReadyToClassify(StatusEntry& entry) {
	return (entry.getRange() == StatusEntry::RANGE_VALIDATED &&
			entry.getClass() == StatusEntry::_NOT_CLASSIFY &&
			(entry.getFlow() == StatusEntry::FLOW_UNSET ||
				entry.getFlow() == StatusEntry::FLOW_PENDING_REUSE));
}

void	StatusManager::_validateCodeRange() {
	StatusEntry&	entry = _statusQueue.front();

	if (!entry.isInRange()) {
		entry.setRange(StatusEntry::_NOT_STATUS_CODE);
	}
	entry.setRange(StatusEntry::RANGE_PENDING);
}

void	StatusManager::_findResponseSource() {
	StatusEntry&	entry = _statusQueue.front();
	// static int		fallback = 0;

	if (entry.getRange() == StatusEntry::RANGE_PENDING) {
		if (_validateWithSources())
			return ;
	}
	_fallbackToInternalError(entry);	// FIXME(@mina): check fall back logic
}

void	StatusManager::_fallbackToInternalError(StatusEntry& entry) {
	entry.setCode(500);						// TODO(@mina): status code 500 or ? -> @kev
	entry.setErrorSource(StatusEntry::_SRC_FALLBACK_INTERNAL);
	entry.setRange(StatusEntry::RANGE_VALIDATED);
}

bool	StatusManager::_validateWithSources() {
	StatusEntry&	entry = _statusQueue.front();
	int				statusCode = entry.getCode();

	/**
	 *	FIXME(@mina): should talk to kev to see where I should refer to the Error class. -> @kev
	 *	 can be from...
	 *		- Config / Router : side
	 *		- Request
	 *		- StatusManager
	 */

	Error			error = _error;

	if (!_request) {
		if (_error.getErrorStr1(statusCode) == "None") {
			return (false);
		}
		return (_returnSource(entry, StatusEntry::SRC_STATIC_RESPONSE));
	}

	const std::map<int, std::string> errorPagesMap = _request->getConfig()->getErrorPages();
	if (errorPagesMap.find(statusCode) == errorPagesMap.end()) {
		if (_error.getErrorStr1(statusCode) == "None") {
			return (false);
		}
		return (_returnSource(entry, StatusEntry::SRC_ERROR_CLASS));
	}
	return (_returnSource(entry, StatusEntry::SRC_SERVER_CONFIG));
}

bool	StatusManager::_returnSource(StatusEntry& entry, StatusEntry::e_source src ) {
	entry.setErrorSource(src);
	entry.setRange(StatusEntry::RANGE_VALIDATED);
	return (true);
}

bool	StatusManager::_validateWithMap(const std::map<int, std::string>* refMap, StatusEntry::e_source refType) {
	StatusEntry&	entry = _statusQueue.front();
	int				code = entry.getCode();

	if (refMap->find(code) != refMap->end()) {
		entry.setErrorSource(refType);
		entry.setRange(StatusEntry::RANGE_VALIDATED);
		return (true);
	}
	return (false);
}

void	StatusManager::_autoSetClasses(StatusEntry& entry) {
		entry.setClass(static_cast<StatusEntry::e_classes>(_statusQueue.front().getCode() / 100));
}

StatusQueue&	StatusManager::getStatusQueue() {
	return (_statusQueue);
}

const StatusQueue& StatusManager::getStatusQueue() const {
	return (_statusQueue);
}