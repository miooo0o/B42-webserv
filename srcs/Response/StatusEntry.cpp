#include "StatusEntry.hpp"

////////////////////////////////////////////////////////////////////////////////

StatusEntry::StatusEntry()
: _code(42), _range(RANGE_PENDING), _class(_NOT_CLASSIFY),
  _exposed(false), _scenariosMap(REF_STATIC_MAP), _flow(FLOW_UNSET), _error(NO_ERROR) {
}

StatusEntry::StatusEntry(int code)
: _code(code), _range(RANGE_PENDING), _class(_NOT_CLASSIFY),
  _exposed(false), _scenariosMap(REF_STATIC_MAP), _flow(FLOW_UNSET), _error(NO_ERROR) {
}

////////////////////////////////////////////////////////////////////////////////
/* boolean */

bool	StatusEntry::isInClassRange() const {
	return (_code >= _class * 100 && _code < _class * 100 + 100);
}

bool	StatusEntry::isInRange() const {
	return (_code >= 100 && _code < 600);
}

bool	StatusEntry::isExposed() const {
	return (_exposed);
}

////////////////////////////////////////////////////////////////////////////////
/* getter */

int	StatusEntry::getCode() const {
	return (_code);
}

StatusEntry::e_range StatusEntry::getRange() const {
	return (_range);
}

StatusEntry::e_classes	StatusEntry::getClass() const {
	return (_class);
}

StatusEntry::e_reference	StatusEntry::getMapReference() const {
	return (_scenariosMap);
}

StatusEntry::e_flow	StatusEntry::getFlow() const {
	return (_flow);
}

StatusEntry::e_error	StatusEntry::getError() const {
  return (_error);
}

/* setter */

void	StatusEntry::setCode(const int code) {
	_code = code;
}

void	StatusEntry::setRange(const e_range range) {
	_range = range;
}

void	StatusEntry::setClass(const e_classes entryClass) {
	_class = entryClass;
}

void	StatusEntry::setExposed(const bool res) {
	_exposed = res;
}

void	StatusEntry::setMapReference(const e_reference ref) {
	_scenariosMap = ref;
}

void	StatusEntry::setFlow(StatusEntry::e_flow f) {
	_flow = f;
}

void	StatusEntry::setError(StatusEntry::e_error e) {
	_error = e;
}

////////////////////////////////////////////////////////////////////////////////
// operator
////////////////////////////////////////////////////////////////////////////////
std::string toString(StatusEntry::e_classes e);
std::string toString(StatusEntry::e_range e);
std::string toString(StatusEntry::e_flow e);
std::string toString(StatusEntry::e_reference e);
std::string toString(StatusEntry::e_error e);

std::ostream&	operator<<(std::ostream& os, const StatusEntry& entry) {
	os << "[StatusEntry]\n";
	os << "  Code            : " << entry.getCode() << "\n";
	os << "  Class           : " << toString(entry.getClass()) << "\n";
	os << "  Validation      : " << toString(entry.getRange()) << "\n";
	os << "  Flow            : " << toString(entry.getFlow()) << "\n";
	os << "  Exposed         : " << (entry.isExposed() ? "true" : "false") << "\n";
	os << "  Map Reference   : " << toString(entry.getMapReference()) << "\n";
	os << "  Error           : " << toString(entry.getError()) << "\n";
	return (os);
}


std::string toString(StatusEntry::e_classes e) {
	switch (e) {
		case StatusEntry::INFORMATIONAL:  return "INFORMATIONAL";
		case StatusEntry::SUCCESSFUL:     return "SUCCESSFUL";
		case StatusEntry::REDIRECTION:    return "REDIRECTION";
		case StatusEntry::CLIENT_ERROR:   return "CLIENT_ERROR";
		case StatusEntry::SERVER_ERROR:   return "SERVER_ERROR";
		case StatusEntry::_NOT_CLASSIFY:  return "_NOT_CLASSIFY";
	}
	return ("UNKNOWN_CLASS");
}

std::string toString(StatusEntry::e_range e) {
	switch (e) {
		case StatusEntry::RANGE_PENDING:     return "RANGE_PENDING";
		case StatusEntry::RANGE_VALIDATED:   return "RANGE_VALIDATED";
		case StatusEntry::_RANGE_UNKNOWN:    return "_RANGE_UNKNOWN";
		case StatusEntry::_OUT_OF_RANGE:     return "_OUT_OF_RANGE";
		case StatusEntry::_NOT_STATUS_CODE:  return "_NOT_STATUS_CODE";
	}
	return ("UNKNOWN_RANGE");
}

std::string toString(StatusEntry::e_flow e) {
	switch (e) {
		case StatusEntry::FLOW_UNSET:        	return "FLOW_UNSET";
		case StatusEntry::FLOW_READY:        	return "FLOW_READY";
		case StatusEntry::FLOW_PENDING_REUSE:	return "FLOW_NEED_UPDATE";
		case StatusEntry::FLOW_PROCESSED:		return "FLOW_PROCESSED";
	}
	return ("UNKNOWN_FLOW");
}

std::string toString(StatusEntry::e_reference e) {
	switch (e) {
		case StatusEntry::REF_STATIC_MAP:   		return "REF_STATIC_MAP";
		case StatusEntry::REF_SERVER_CONFIG:		return "REF_SERVER_CONFIG";
		case StatusEntry::_REF_FALLBACK_INTERNAL:	return "_REF_FALLBACK_INTERNAL";
	}
	return ("UNKNOWN_REF");
}

std::string toString(StatusEntry::e_error e) {
	switch (e) {
		case StatusEntry::NO_ERROR:			return "NO_ERROR";
		case StatusEntry::ERROR_EVALUATION:	return "NO_ERROR";
	}
	return ("UNKNOWN_REF");
}