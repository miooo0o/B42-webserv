#include "Response.hpp"
#include "Entries.hpp"
#include "Request.hpp"
#include "EntryObserver.hpp"
#include "ResponseState.hpp"

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
Response::Response(Request& request, Entries* entries)
	: _request(request), _entries(entries), _headers(), _body() {
	_serverMap = _request.getServerMap();
	_entries->addObserver(this);
	this->onEntryChanged();
}

Response::~Response() {
	if (_entries) {
		_entries->removeObserver(this);
	}
	_cleanState();
}

/**
 * @brief Handles updates when an entry status changes.
 *
 * This function is triggered whenever the `Entries` queue undergoes changes.
 * It calls `updateState()` to ensure the response reflects the latest entry status.
 */
void    Response::onEntryChanged() {
	updateState();
}

/**
 * @brief Updates the ResponseState based on the latest entry status.
 *
 * This function evaluates the entry's status and determines the appropriate state subclass.
 * If the entry is successfully validated, it assigns a new state (`SuccessState`, 
 * `ErrorState`, `RedirectState`, etc.). Otherwise, it throws an exception.
 *
 * @throws `std::logic_error` if the status code is out of range or queue is unprocessed.
 */
void	Response::updateState() {
	try {
		_prepareForEvaluation();
		if (!_entries->eval(_serverMap))
			throw std::logic_error("Queue is not ready.");

		Entry::e_classes statusClass = _entries->getClass();

		if (_state && _shouldReuseState(statusClass)) {
			_entries->resetEntryWith(_entries->getEntry());
			_entries->eval(_serverMap);
			return ;
		}
		_replaceState(statusClass);
	}
	catch (std::exception& e) {
		std::cerr << "Response: Exception in updateState: " << e.what() << std::endl;
		if (!_state) {
			_cleanState();
			_state = new ErrorState(_request, *_entries);
		}
	}
}

void	Response::_prepareForEvaluation() {
	if (_entries->getQueueStatus() == Entries::QUEUE_FULL)
		_entries->setQueLevel(Entries::QUEUE_PENDING);
}

bool	Response::_shouldReuseState(Entry::e_classes statusClass) const {
	return (_state->getClass() != Entry::_NOT_CLASSIFY &&
			_state->getClass() == statusClass);
}

void	Response::_replaceState(Entry::e_classes statusClass) {
	_cleanState();
	switch (statusClass) {
		case Entry::INFORMATIONAL:
			_state = new InformationalState(_request, *_entries);
			break;
		case Entry::SUCCESSFUL:
			_state = new SuccessState(_request, *_entries);
			break;
		case Entry::REDIRECTION:
			_state = new RedirectState(_request, *_entries);
			break;
		case Entry::CLIENT_ERROR:
		case Entry::SERVER_ERROR:
			_state = new ErrorState(_request, *_entries);
			break;
		default:
			throw std::logic_error("Status code is not in range.");
	}
}

void    Response::_cleanState() {
	if (_state)
		delete _state;
	_state = NULL;
}
