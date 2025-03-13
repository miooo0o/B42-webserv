#include "Entries.hpp"
#include "Reqeust.hpp"
#include "ResponseState.hpp"

////////////////////////////////////////////////////////////////////////////////

Status::Status()
: _code(42), _valStatus(STATUS_PENDING), _class(_NOT_CLASSIFY),
  _exposed(false), _ref(REF_STATIC_MAP) {
}

Status::Status(int code)
: _code(code), _valStatus(STATUS_PENDING), _class(_NOT_CLASSIFY), 
  _exposed(false), _ref(REF_STATIC_MAP) {
	if (isInRange()) {
		eval();
	}
}

////////////////////////////////////////////////////////////////////////////////

bool	Status::isInClassRange() const {
	return (_code >= _class * 100 && _code < _class * 100 + 100);
}

bool	Status::isInRange() const {
	return (_code >= 100 && _code < 600);
}

void	Status::eval() {
	/* TODO: check server side map for status coode */
	// _ref = checkReference();
	_valStatus = _validate();
	if (_valStatus != STATUS_VALIDATED) {
		return ;
	}
	_class = _classify();
	_exposed = _class != INFORMATIONAL;
}

////////////////////////////////////////////////////////////////////////////////

Status::e_validate Status::_validate() {
	if (!isInRange()) {
		return (_STATUS_NOT);
	}
	if (_ref == REF_STATIC_MAP) {
		std::map<int, std::string>& referenceMap = ResponseState::getScenarios();
		if (referenceMap.find(_code) == referenceMap.end()) {
			return (_STATUS_UNKNOWN);
		}
	} else { //  TODO: REF_SERVER_CONFIG
		return (_STATUS_UNKNOWN);
	}
	return (STATUS_VALIDATED);
}

Status::e_classes Status::_classify() {
	return (static_cast<Status::e_classes>(_code / 100));
}


////////////////////////////////////////////////////////////////////////////////

int	Status::getCode() const {
	return (_code);
}

Status::e_validate Status::getValStatus() const {
	return (_valStatus);
}

Status::e_classes	Status::getClass() const {
	return (_class);
}

bool    Status::isValidated() const {
	return (_valStatus == STATUS_VALIDATED ? true : false);
}

bool	Status::isExposed() const {
	return (_exposed);
}

////////////////////////////////////////////////////////////////////////////////
// Entries
////////////////////////////////////////////////////////////////////////////////

Entries::Entries(Request& request) {
	_initEntry(request);
	if (!_readFromRequest) {
		replace(Status(400));
	}
}

Entries::~Entries() {}

void    Entries::_initEntry(Request& reqest) {
	int requestStatusCode = 404; // request.getReqeustError();
	push_back(Status(requestStatusCode));
}

void    Entries::replace(Status status) {
	pop_back();
	if (_state == QUEUE_EMPTY)
		push_back(status);
}

void	Entries::push_back(Status status) {
	if (_state < QUEUE_EMPTY)
		return ;
	if (_state > QUEUE_EMPTY) {
		setState(_QUEUE_OVERFLOW); return ;
	}
	_entries.push_back(status);
}

void	Entries::pop_back() {
	if (_entries.empty()) {
		setState(_QUEUE_UNDERFLOW); return ;
	}
	_entries.pop_back();
}

Status	Entries::get_back() {
	if (_state <= QUEUE_EMPTY) {
		return *(_entries.end());
	}
	return *(_entries.end() - 1);
}