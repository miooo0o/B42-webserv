#include "Entries.hpp"
#include "Request.hpp"
#include "ResponseState.hpp"

////////////////////////////////////////////////////////////////////////////////

Entry::Entry()
: _code(42), _entryVal(ENTRY_PENDING), _class(_NOT_CLASSIFY),
  _exposed(false), _scenariosMap(REF_STATIC_MAP) {
}

Entry::Entry(int code)
: _code(code), _entryVal(ENTRY_PENDING), _class(_NOT_CLASSIFY), 
  _exposed(false), _scenariosMap(REF_STATIC_MAP) {
}

////////////////////////////////////////////////////////////////////////////////
/* boolean */

bool	Entry::isInClassRange() const {
	return (_code >= _class * 100 && _code < _class * 100 + 100);
}

bool	Entry::isInRange() const {
	return (_code >= 100 && _code < 600);
}

bool	Entry::isExposed() const {
	return (_exposed);
}

////////////////////////////////////////////////////////////////////////////////
/* getter */

int	Entry::getCode() const {
	return (_code);
}

Entry::e_entry_val Entry::getValidateStatus() const {
	return (_entryVal);
}

Entry::e_classes	Entry::getClass() const {
	return (_class);
}

Entry::e_reference	Entry::getMapReference() const {
	return (_scenariosMap);
}

/* setter */

void	Entry::setCode(int code) {
	_code = code;
}

void	Entry::setValidateStatus(e_entry_val status) {
	_entryVal = status;
}

void	Entry::setClass(e_classes entryClass) {
	_class = entryClass;
}

void	Entry::setExposed(bool res) {
	_exposed = res;
}

void		Entry::setMapReference(e_reference ref) {
	_scenariosMap = ref;
}


////////////////////////////////////////////////////////////////////////////////
// Entries
////////////////////////////////////////////////////////////////////////////////

Entries::Entries(Request& request) 
: _queueLevel(QUEUE_EMPTY) {
	_initEntry(request);
	if (!_readFromRequest) {
		replace(Entry(400));
	}
}

Entries::~Entries() {}

////////////////////////////////////////////////////////////////////////////////


void	Entries::_notifyObservers() {
	for (std::vector<EntryObserver*>::iterator it = _observers.begin();
		 it != _observers.end(); ++it)
	{
		(*it)->onEntryChanged();
	}
}

void	Entries::addObserver(EntryObserver* observer) {
	_observers.push_back(observer);
}

void	Entries::removeObserver(EntryObserver* observer) {
	for (std::vector<EntryObserver*>::iterator it = _observers.begin(); it != _observers.end(); ++it) {
		if (*it == observer) {
			_observers.erase(it);
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////


void    Entries::_initEntry(Request& reqest) {
	if (!_entries.empty()) {
		throw std::logic_error("Entries queue is not empty during initialization.");
	}
	int requestStatusCode = reqest.getCode(); // request.getReqeustError();
	push_back(Entry(requestStatusCode));
	if (QUEUE_HAS_DATA)
		_readFromRequest = true;
}

void    Entries::replace(Entry status) {
	pop_front();
	if (_queueLevel == QUEUE_EMPTY)
		push_back(status);
}

void	Entries::push_back(Entry status) {

	if (_queueLevel != QUEUE_EMPTY) {
		_recodeStatus(status);
		if (QUEUE_HAS_ERROR) return ;
		if (QUEUE_HAS_DATA) {
			setState(_QUEUE_OVERFLOW); return ;
		}
	}
	_entries.push_back(status);
	setState(QUEUE_FULL);
	_notifyObservers();
}

void	Entries::pop_front() {
	if (_entries.empty()) {
		setState(_QUEUE_UNDERFLOW); return ;
	}
	_recodeStatus();
	_entries.pop_front();
	setState((_entries.empty() ? QUEUE_EMPTY : _QUEUE_ERROR));
}
////////////////////////////////////////////////////////////////////////////////
/* Logger support methods : need to fix if Logger class is implemented */

void	Entries::_recodeStatus() {
	_enforceLogSizeLimit();
	_log.push_back(_entries.front()); 
}

void	Entries::_recodeStatus(Entry& status) {
	_enforceLogSizeLimit();
	_log.push_back(_entries.front()); 
}

// TODO: if log is full, move() to Log class -> export to file
void	Entries::_enforceLogSizeLimit() {
	static int warningCount = 0;
	while (_log.size() > MAX_LOG_QUEUE_SIZE) {
		_log.pop_front();
		warningCount++;
	}
	if (warningCount > 0) {
		std::cerr << "Warning: Entries.log exceeded limit. Cleared " << warningCount << " old entries.\n";
		warningCount = 0;
	} 
}

////////////////////////////////////////////////////////////////////////////////

/**
 * 
 */
bool Entries::ready() {
	if (_queueLevel == QUEUE_FULL) {
		std::cerr << "debug: _entries is full, eval() needs to be called externally." << std::endl;
		return (false);
	}
	if (_queueLevel == QUEUE_PENDING) {
		return (true);
	}
	std::cerr << "debug: _entries cannot proceed. Manual intervention needed." << std::endl;
	return (false);
}

/**
 * @brief Evaluates the current Entry status based on predefined validation rules.
 *
 * This function ensures that the entry is ready for validation by checking `ready()`. 
 * It then performs a range validation, searches for a reference in server maps, 
 * and finally classifies the entry based on its HTTP status code.
 *
 * @param serverScenarios A map containing server-defined HTTP response codes.
 * @return `true` if the entry is successfully validated and classified; `false` otherwise.
 */
bool	Entries::eval(const std::map<int, std::string>& serverScenarios) {
	if (!ready()) return (false);
	_validateCodeRange();
	_findCodeReference(serverScenarios);
	if (getEntry().getValidateStatus() == Entry::ENTRY_VALIDATED) {
		_classify();
		getEntry().setExposed(getEntry().getClass() != Entry::INFORMATIONAL);
		_queueLevel = QUEUE_PENDING;
		return (true);
	}
	return (false);
}

/**
 * @brief Validates whether the current entry's status code is within a valid HTTP range.
 *
 * This method checks if the entry's status code is within the HTTP range (100-599).
 * If it is not, the status is set to `_ENTRY_NOT`. Otherwise, it remains in `ENTRY_PENDING`.
 */
void	Entries::_validateCodeRange() {
	if (!getEntry().isInRange()) {
		getEntry().setValidateStatus(Entry::_ENTRY_NOT);
	}
	getEntry().setValidateStatus(Entry::ENTRY_PENDING);
}

/**
 * @brief Searches for the status code in the predefined maps (server-side and static).
 *
 * This function first looks into the server-side response map. If not found, it searches 
 * the static scenario map (`ResponseState::getScenarios`). If neither contain the code, 
 * after two failed search attempts, it assigns a 500 Internal Server Error status.
 *
 * @param serverSideMaps A map of server-defined response codes.
 */
void	Entries::_findCodeReference(const std::map<int, std::string>& serverSideMaps) {
	static int searchAttempts = 0;

	if (getEntry().getValidateStatus() != Entry::ENTRY_PENDING)
		return;
	int code = getEntry().getCode();
	if (_validateWithMap(serverSideMaps, Entry::REF_SERVER_CONFIG))
		return;
	else if (_validateWithMap(ResponseState::getScenarios(), Entry::REF_STATIC_MAP))
		return;
	if (++searchAttempts > 1) {
		getEntry().setMapReference(Entry::_500_NOT_FOUND);
		getEntry().setValidateStatus(Entry::ENTRY_VALIDATED);
		searchAttempts = 0;
		return;
	}
	getEntry().setValidateStatus(Entry::_ENTRY_UNKNOWN);
}

bool	Entries::_validateWithMap(const std::map<int, std::string>& refMap, Entry::e_reference refType) {
	int code = getEntry().getCode();
	if (refMap.find(code) != refMap.end()) {
		getEntry().setMapReference(refType);
		getEntry().setValidateStatus(Entry::ENTRY_VALIDATED);
		return (true);
	}
	return (false);
}

void	Entries::_classify() {
	getEntry().setClass(static_cast<Entry::e_classes>(getEntry().getCode() / 100));
}

////////////////////////////////////////////////////////////////////////////////
/* getter */

Entry	Entries::getEntry() const {
	if (_entries.empty()) {
		if (QUEUE_IS_ACTIVE || !QUEUE_HAS_ERROR)
			throw std::logic_error("Entries queue is empty, but Queue didn't catch the error.");
		else
			throw std::logic_error("Entries queue is empty.");
	}
	else if (_entries.size() > 1) {

		if (QUEUE_IS_ACTIVE || !QUEUE_HAS_ERROR)
			throw std::logic_error("Entries queue is overflow, but Queue didn't catch the error.");
		else
			throw std::logic_error("Entries queue is overflow");
	}
	return (_entries.front());
}

Entry	Entries::getEntry() {
	if (_entries.empty()) {
		if (QUEUE_IS_ACTIVE || !QUEUE_HAS_ERROR)
			throw std::logic_error("Entries queue is empty, but Queue didn't catch the error.");
		else
			throw std::logic_error("Entries queue is empty.");
	}
	else if (_entries.size() > 1) {

		if (QUEUE_IS_ACTIVE || !QUEUE_HAS_ERROR)
			throw std::logic_error("Entries queue is overflow, but Queue didn't catch the error.");
		else
			throw std::logic_error("Entries queue is overflow");
	}
	return (_entries.front());
}


Entries::e_que_entries	Entries::getQueueStatus() const {
	return (_queueLevel);
}

int	Entries::getCode() const {
	return (getEntry().getCode());
}

Entry::e_entry_val	Entries::getStatus() const {
	return (getEntry().getValidateStatus());
}

Entry::e_classes	Entries::getClass() const {
	return (getEntry().getClass());
}

Entry::e_reference	Entries::getMapRef() const {
	return (getEntry().getMapReference());
}


////////////////////////////////////////////////////////////////////////////////
/* setter */

void	Entries::setState(Entries::e_que_entries queueLevel) {
	_queueLevel = queueLevel;
}

void	Entries::setCode(int code) {
	return (getEntry().setCode(code));
}

void	Entries::setStatus(Entry::e_entry_val entryValLevel) {
	return (getEntry().setValidateStatus(entryValLevel));
}
void	Entries::setClass(Entry::e_classes entryClass) {
	return (getEntry().setClass(entryClass));
}
void	Entries::setExposed(bool res) {
	return (getEntry().setExposed(res));
}
void	Entries::setMapRef(Entry::e_reference ref) {
	return (getEntry().setMapReference(ref));
}

////////////////////////////////////////////////////////////////////////////////
/* has / is */
 
bool	Entries::hasSingleEntry() const {
	return (_entries.size() == 1);
}

bool	Entries::isPending() const {
	return (getEntry().getValidateStatus() == Entry::ENTRY_PENDING);
}

bool	Entries::isExposed() const {
	return (getEntry().isExposed());
}

bool	Entries::isInRange() const {
	return (getEntry().isInRange());
}
bool	Entries::isInClassRange() const {
	return (getEntry().isInClassRange());
}
bool	Entries::isValidated() const {
	return (getEntry().getValidateStatus() == Entry::ENTRY_VALIDATED);
}
