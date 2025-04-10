/* 
#include "StatusQueue.hpp"
#include "StatusManager.hpp"
#include "Request.hpp"
#include "ResponseState.hpp"
#include "StatusEntry.hpp"
 */

#include "../../includes/StatusQueue.hpp"
#include "../../includes/StatusManager.hpp"
#include "../../includes/Request.hpp"
#include "../../includes/ResponseState.hpp"
#include "../../includes/StatusEntry.hpp"

////////////////////////////////////////////////////////////////////////////////
// StatusManager
////////////////////////////////////////////////////////////////////////////////

StatusManager::StatusManager(const Request& request) {
	_initEntry(request);
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
		 it != _observers.end(); ++it)
	{
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


void    StatusManager::_initEntry(const Request& request) {
	if (!_statusQueue.empty()) {
		throw std::logic_error("Entries queue is not empty during initialization.");
	}
	int requestStatusCode = request.getResponseCode();
	_statusQueue.push(StatusEntry(requestStatusCode));
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
	_statusQueue.front().setMapReference(StatusEntry::REF_STATIC_MAP);
	_statusQueue.front().setFlow(StatusEntry::FLOW_PENDING_REUSE);
}

////////////////////////////////////////////////////////////////////////////////

bool StatusManager::ready() {
	return (!_statusQueue.empty());
}


bool	StatusManager::eval(const std::map<int, std::string>& serverScenarios) {
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

void	StatusManager::_findCodeReference(const std::map<int, std::string>& serverSideMaps) {
	StatusEntry&	entry = _statusQueue.front();
	if (entry.getRange() != StatusEntry::RANGE_PENDING)
		return;
	if (_validateWithMap(serverSideMaps, StatusEntry::REF_SERVER_CONFIG) ||
		_validateWithMap(ResponseState::getScenarios(), StatusEntry::REF_STATIC_MAP)) {
		return;
	}
	_fallbackToInternalError(entry); // FIXME: status code check need @kevin
}

void	StatusManager::_fallbackToInternalError(StatusEntry& entry) {
	entry.setCode(500);
	entry.setMapReference(StatusEntry::_REF_FALLBACK_INTERNAL);
	entry.setRange(StatusEntry::RANGE_VALIDATED);
}

bool	StatusManager::_validateWithMap(const std::map<int, std::string>& refMap, StatusEntry::e_reference refType) {
	StatusEntry&	entry = _statusQueue.front();
	int				code = entry.getCode();

	if (refMap.find(code) != refMap.end()) {
		entry.setMapReference(refType);
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