#include "Response.hpp"
#include "StatusQueue.hpp"
#include "Request.hpp"
#include "StatusManager.hpp"
#include "ResponseState.hpp"
#include "StatusEntry.hpp"

/**
* @brief Constructs a regular Response object tied to a Request
* @param request Pointer to the Request object (must not be NULL)
* @throws std::logic_error if request is 'NULL`
* @note For generic use. Request cannot be `NULL` as this is not a static response.
*/
Response::Response(Request* request)
: _request(request), _manager(_request), _state(),
  _serverMap(request && request->getConfig() ?
  	&(request->getConfig()->getErrorPages()) : NULL),
  _call_static(false) {
	if (_request == NULL)
		throw std::logic_error("Response has Request param, but Request is NULL");
	_createResponse();
}

/**
* @brief Constructs a static Response object
* @param statusCode HTTP status code for the response
* @note CAUTION: Only used for static responses. Sets request and serverMap to NULL,
*       _call_static to true, and initializes with ErrorState that cannot be modified.
*/
Response::Response(int statusCode)
: _request(NULL), _manager(statusCode), _state(new ErrorState(NULL, _manager)),
  _serverMap(NULL), _call_static(true) {
	_createResponse();
}

bool	Response::_createResponse() {
	_manager.addObserver(this);
	this->Response::_onEntryChanged();
}

Response::~Response() {
	_manager.removeObserver(this);
	_cleanState();
}

////////////////////////////////////////////////////////////////////////////////
// Public method
////////////////////////////////////////////////////////////////////////////////

void	Response::addStatusCode(int newCode) {
	StatusEntry& oldEntry = _manager.getStatusQueue().front();
	if (_state && StatusEntry::same_class(oldEntry, newCode)) {
		_manager.reuseEntry(newCode);
		_onEntryChanged();
	} else {
		_manager.archiveAll();
		_manager.push(StatusEntry(newCode));
	}
}

StatusManager	Response::getStatusManager() const {
	return (_manager);
}

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Handles updates when an entry status changes.
 *
 * This function is triggered whenever the `Entries` queue undergoes changes.
 * It calls `updateState()` to ensure the response reflects the latest entry status.
 */
void    Response::_onEntryChanged() {
	_syncState();
	_syncCurrentEntry();
}

void	Response::_syncState() {
	try {
		StatusEntry&	target = _manager.getStatusQueue().front();

		if (_handleFlowUpdate(target)) return;
		if (!_manager.eval(_serverMap))
			throw std::logic_error("Queue is not ready.");
		if (!_call_static)
			_assignNewState(target.getClass());
	}
	catch (std::exception& e) {
		_handleUpdateException(e); // TODO: change to Error class
	}
}

bool	Response::_handleFlowUpdate(StatusEntry& target) {
	if (target.getFlow() == StatusEntry::FLOW_PENDING_REUSE) {
		_manager.eval(_serverMap);
		if (target.getFlow() == StatusEntry::FLOW_READY)
			target.setFlow(StatusEntry::FLOW_PROCESSED);
		return (true);
	}
	return (false);
}

void	Response::_assignNewState(StatusEntry::e_classes statusClass) {
	_cleanState();
	switch (statusClass) {
		case StatusEntry::INFORMATIONAL:
			_state = new InformationalState(_request, _manager);
			break;
		case StatusEntry::SUCCESSFUL:
			_state = new SuccessState(_request, _manager);
			break;
		case StatusEntry::REDIRECTION:
			_state = new RedirectState(_request, _manager);
			break;
		case StatusEntry::CLIENT_ERROR:
		case StatusEntry::SERVER_ERROR:
			_state = new ErrorState(_request, _manager);
			break;
		default:
			throw std::logic_error("Status code is not in range.");
	}
	_manager.getStatusQueue().front().setFlow(StatusEntry::FLOW_PROCESSED);
}

void    Response::_cleanState() {
	if (_state)
		delete _state;
	_state = NULL;
}

void	Response::_handleUpdateException(const std::exception& e) {
	std::cerr << "Response: Exception in updateState: " << e.what() << std::endl;
	if (!_state) {
		_cleanState();
		_state = new ErrorState(_request, _manager);
	}
}

void	Response::_syncCurrentEntry() {
	if (_state && _manager.ready())
		_state->setCurrentEntry(&_manager.getStatusQueue().front());
}