#include "Response.hpp"
#include "StatusQueue.hpp"
#include "Request.hpp"
#include "StatusManager.hpp"
#include "ResponseState.hpp"
#include "StatusEntry.hpp"

/**
 * @brief Constructs a Response object with a request and an entry queue.
 *
 * This constructor initializes the response with a reference to the request and the 
 * `Entries` object. It also registers the response as an observer to `Entries`, allowing 
 * automatic state updates when the queue changes.
 *
 * @param request The HTTP request associated with this response.
 * @param entries A pointer to the `Entries` queue for status management.
 */
Response::Response(Request& request)
	: _request(request), _manager(request), _headers(), _body() {
	_serverMap = _request.getServerMap();
	_manager.addObserver(this);
	this->_onEntryChanged();
}

Response::~Response() {
	_manager.removeObserver(this);
	_cleanState();
}

////////////////////////////////////////////////////////////////////////////////
// Public method
////////////////////////////////////////////////////////////////////////////////

void		Response::addStatusCode(int newCode) {
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
}

void	Response::_syncState() {
	try {
		StatusEntry&	target = _manager.getStatusQueue().front();

		if (_handleFlowUpdate(target)) return;
		if (!_manager.eval(_serverMap))
			throw std::logic_error("Queue is not ready.");
		_assignNewState(target.getClass());
	}
	catch (std::exception& e) {
		_handleUpdateException(e);
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