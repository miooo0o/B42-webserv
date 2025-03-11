#include "Response.hpp"
#include "ResponseState.hpp"

/* Constructor */
Response::Response(int statusCode) : _state(NULL) {
    setState(statusCode);
}

/* Destructor */
Response::~Response() {
    if (_state) {
        delete _state;
    }
}

/* Set State Dynamically */
void    Response::setState(int statusCode) {
    if (_state) {
        delete _state;
    }
    _state = createState(statusCode);
}

ResponseState*  Response::createState(int statusCode) {
    ResponseState::e_class _class = ResponseState(statusCode).getStatusClass();

    switch (_class) {
        case ResponseState::SUCCESSFUL:
            return new SuccessState(statusCode);
        case ResponseState::REDIRECTION:
            return new RedirectState("https://example.com");
        case ResponseState::CLIENT_ERROR:
        case ResponseState::SERVER_ERROR:
            return new ErrorState(statusCode);
        default:
            return new ErrorState(500);
    }
}