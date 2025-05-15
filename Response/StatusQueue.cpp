#include "StatusQueue.hpp"
#include "StatusManager.hpp"
#include "StatusEntry.hpp"

////////////////////////////////////////////////////////////////////////////////

StatusQueue::StatusQueue() : _statusQueue(), _log() {}

////////////////////////////////////////////////////////////////////////////////

StatusEntry&	StatusQueue::front() {
	if (_statusQueue.empty()) {
		throw std::logic_error("StatusQueue: queue is empty");
	}
	return (_statusQueue.front());
}

const StatusEntry&	StatusQueue::front() const {
	if (_statusQueue.empty()) {
		throw std::logic_error("StatusQueue: queue is empty");
	}
	return (_statusQueue.front());
}

void	StatusQueue::push(const StatusEntry& entry) {
	_statusQueue.push_back(entry);
}

void	StatusQueue::pop() {
	if (_statusQueue.empty()) {
		throw std::logic_error("Queue is empty");
	}
	recordToLog();
	_statusQueue.pop_front();
}

void	StatusQueue::replace(const StatusEntry& entry) {
	if (_statusQueue.empty())
		throw std::logic_error("StatusQueue: queue is empty");
	pop();
	push(entry);
}

////////////////////////////////////////////////////////////////////////////////

void	StatusQueue::archiveProcessed() {
	for (std::deque<StatusEntry>::iterator it = _statusQueue.begin(); it != _statusQueue.end(); ) {
		if (it->getFlow() == StatusEntry::FLOW_PROCESSED) {
			enforceLogLimit(MAX_LOG_QUEUE_SIZE);
			_log.push_back(*it);
			it = _statusQueue.erase(it);
		} else {
			++it;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void	StatusQueue::recordToLog() {
	if (!_statusQueue.empty()) {
		enforceLogLimit(MAX_LOG_QUEUE_SIZE);
		_log.push_back(_statusQueue.front());
	}
}

void StatusQueue::enforceLogLimit(std::size_t maxSize) {
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

bool StatusQueue::empty() const {
	return (_statusQueue.empty());
}

const std::deque<StatusEntry>& StatusQueue::getLog() const {
	return (_log);
}

const std::deque<StatusEntry>& StatusQueue::getstatusQueue() const {
	return (_statusQueue);
}