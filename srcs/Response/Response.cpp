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
    try {
        Entry entry = _entries->getEntry();
    }
    catch (std::exception& ex) {
        std::cerr << "Response: Exception: " << ex.what() << std::endl;
    }
}