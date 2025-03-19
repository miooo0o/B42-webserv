#include "Response.hpp"
#include "Request.hpp"
#include "Entries.hpp"
#include "EntryObserver.hpp"

/* Constructor */
Response::Response(Request& request, Entries* entries)
    : _request(request), _entries(entries), _headers(), _body() {
    _entries.addObserver(this);
}

void    Response::onEntryChanged() {
    updateState();
}

void Response::updateState() {
    try {
        if (_entries->eval(_serverMap));
            throw std::logic_error("Queue is not ready.");
        Entry::e_classes statusClass = _entries->getClass();
        switch (statusClass) {
            case Entry::e_classes::INFORMATIONAL: {
                if (_state) delete _state;
                _state = new InformationalState(_request);
                break;
            }
            case Entry::e_classes::SUCCESSFUL: {
                if (_state) delete _state;
                _state = new SuccessState(_request);
                break;
            }
            case Entry::e_classes::REDIRECTION: {
                if (_state) delete _state;
                _state = new RedirectState(_request);
                break;
            }
            case Entry::e_classes::CLIENT_ERROR:
            case Entry::e_classes::SERVER_ERROR: {
                if (_state) delete _state;
                _state = new ErrorState(_request);
                break;
            }
            default: {
                break;
            }
        }
    }
    catch (std::exception& e) {
        std::cerr << "Response: Exception in updateState: " << e.what() << std::endl;
    }
}

Response		Response::to_response() {
    
}