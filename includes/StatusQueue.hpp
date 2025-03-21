#ifndef STATUSQUEUE_HPP
# define STATUSQUEUE_HPP

#include <deque>

/* Define MAX_LOG_QUEUE_SIZE with an appropriate value */
#define MAX_LOG_QUEUE_SIZE 20

#include "StatusEntry.hpp"

class StatusEntry;

class StatusQueue {
private:
	std::deque<StatusEntry>	_statusQueue;
	std::deque<StatusEntry>	_log;

public:
	StatusQueue();

	StatusEntry&		front();
	const StatusEntry&	front() const;
	void				push(const StatusEntry& entry);
	void				pop();
	void				replace(const StatusEntry& entry);

	void				archiveProcessed();


	void				recordToLog();
	void				enforceLogLimit(std::size_t maxSize);


	/* getter, Log */
	const std::deque<StatusEntry>&	getLog() const;
	const std::deque<StatusEntry>&	getstatusQueue() const;

	/* has, is */
	bool			empty() const;

};

#endif //STATUSQUEUE_HPP
