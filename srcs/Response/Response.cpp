#include "Response.hpp"
#include "ResponseState.hpp"
#include "Reqeust.hpp"

/* Constructor */
Response::Response(Request& request) : _request(request), _state(), _headers(), _body() {
}

/* Destructor */
Response::~Response() {
    if (_state) {
        delete _state;
    }
}

/* Set State Dynamically */
void    Response::setState(int code) {
    if (_state) {
        delete _state;
    }
    _state = createState();
}

ResponseState*  Response::createState() {


    
    switch (targetClass) {
        case ResponseState::SUCCESSFUL:
            return new SuccessState(_request);
        case ResponseState::REDIRECTION:
            return new RedirectState(_request);
        case ResponseState::CLIENT_ERROR:
        case ResponseState::SERVER_ERROR:
            return new ErrorState(_request);
        default:
            return new ErrorState(_request);
    }
}

bool    Response::evaluateState() {
    
}
	/*
	
	if (target status code is specified in config's error_page directive) {
    	if (the corresponding error page file exists) {
			return (open & read file -> std::string)
		} else {
			return ( chage to error state )
		}
	} else {
		if (target status is specified in `static map scenarios`) {
			return (generate page)
		} else {
			return ( change to error state)
		}
	}
	
	*/

