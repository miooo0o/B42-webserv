#include "Response.hpp"
#include "Request.hpp"
#include "Entries.hpp"
#include "EntryObserver.hpp"

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
	_entries.addObserver(this);
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
void    Response::updateState() {
	try {
		if (_entries->eval(_serverMap));
			throw std::logic_error("Queue is not ready.");

		Entry::e_classes statusClass = _entries->getClass();
		if (_state && _state->getClass() == statusClass) {
			_state->updateStatus(_entries->getEntry());
		}
		_cleanState();
		switch (statusClass) {
			case Entry::e_classes::INFORMATIONAL: {
				_state = new InformationalState(_request);
				break;
			}
			case Entry::e_classes::SUCCESSFUL: {
				_state = new SuccessState(_request);
				break;
			}
			case Entry::e_classes::REDIRECTION: {
				_state = new RedirectState(_request);
				break;
			}
			case Entry::e_classes::CLIENT_ERROR:
			case Entry::e_classes::SERVER_ERROR: {
				_state = new ErrorState(_request);
				break;
			}
			default: {
				throw std::logic_error("Status code is not in range.")
				break;
			}
		}
	}
	catch (std::exception& e) {
		std::cerr << "Response: Exception in updateState: " << e.what() << std::endl;
		if (!_state) {
			_cleanState();
            _state = new ErrorState(_request);
		}

	}
}

void    Response::_cleanState() {
	if (_state)
		delete _state;
	_state = NULL;
}

#include "Response.hpp"
#include "Request.hpp"
#include "Entries.hpp"
#include "EntryObserver.hpp"

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
	_entries.addObserver(this);
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
void    Response::updateState() {
	try {
		if (_entries->eval(_serverMap));
			throw std::logic_error("Queue is not ready.");

		Entry::e_classes statusClass = _entries->getClass();
		if (_state && _state->getClass() == statusClass) {
			_state->updateStatus(_entries->getEntry());
		}
		_cleanState();
		switch (statusClass) {
			case Entry::e_classes::INFORMATIONAL: {
				_state = new InformationalState(_request);
				break;
			}
			case Entry::e_classes::SUCCESSFUL: {
				_state = new SuccessState(_request);
				break;
			}
			case Entry::e_classes::REDIRECTION: {
				_state = new RedirectState(_request);
				break;
			}
			case Entry::e_classes::CLIENT_ERROR:
			case Entry::e_classes::SERVER_ERROR: {
				_state = new ErrorState(_request);
				break;
			}
			default: {
				throw std::logic_error("Status code is not in range.")
				break;
			}
		}
	}
	catch (std::exception& e) {
		std::cerr << "Response: Exception in updateState: " << e.what() << std::endl;
		if (!_state) {
			_cleanState();
            _state = new ErrorState(_request);
		}

	}
}

void    Response::_cleanState() {
	if (_state)
		delete _state;
	_state = NULL; 
}