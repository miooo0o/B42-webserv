#pragma once

#include <deque>

class Status {
	/**
	 * @brief Status validation flags for the Entries class.
	 */
	enum e_flags {
		FLAG_PENDING         	= 0,	/* Default state, pending validation */
		FLAG_VALIDATED       	= 1,	/* Status code is valid and registered */
		_FLAG_UNKNOWN        	= -1,	/* Code is within valid range but not registered */
		_FLAG_OUT_OF_RANGE   	= -2,	/* Code is outside the HTTP status code range (100-599) */
		_FLAG_NOT_CODE_STATUS	= -3	/* Code is not a valid HTTP status (e.g., negative or too large) */
	};

	/** 
 	* @brief Represents the classification of HTTP status codes into categories.
 	*/
	enum e_classes {
		_NOT_CLASSIFY			= 0,	/* Default unclassified state */
		INFORMATIONAL			= 1,	/* 1xx: Informational responses */
		SUCCESSFUL				= 2,	/* 2xx: Successful responses */
		REDIRECTION				= 3,	/* 3xx: Redirection messages */
		CLIENT_ERROR			= 4,	/* 4xx: Client error responses */
		SERVER_ERROR			= 5 	/* 5xx: Server error responses */
	};

private:
    int			_code;
    e_flags		_flag;
    e_classes	_class;
    bool		_exposed;
	bool		_validate;

public:
    /* Constructor */
    Status();
    Status(int code);

    /* Getter methods */
    int			getCode() const;
    e_flags		getFlag() const;
    e_classes	getClass() const;
    bool		isValidated() const;
	bool		isEmpty() const;

    /* Validation methods ( this.method() ) */
	bool		isExposed() const;
    bool		isStatusCode() const;
    bool		isStatusCodeInClassRange() const;
    e_flags		validateStatusCode();
    e_classes	classifyStatusCode();

    /* Validation methods */
    bool		isStatusCode(int code) const;
    bool		isStatusCodeInClassRange(int code) const;
    e_flags		validateStatusCode(int code);
    e_classes	classifyStatusCode(int code);
};


class Entries {
public:
	/**
	 * @brief Queue status flags to track the current state of the status queue.
	 */
	enum e_state {
		_QUEUE_OVERFLOW	= -3,		/* The queue has exceeded its maximum capacity */
		_QUEUE_UNDERFLOW	= -2,	/* An attempt was made to pop from an empty queue */
		_QUEUE_ERROR		= -1,	/* An error occurred during status processing */
		QUEUE_EMPTY		= 0,		/* The queue is empty */
		QUEUE_PENDING,				/* There are statuses waiting to be processed */
		QUEUE_PROCESSING,			/* A status is currently being processed */
		QUEUE_COMPLETE,				/* The processing of a status is completed */
		QUEUE_FULL					/* The queue is full and cannot accept more statuses */
	};

private:
	std::deque<Status>	_entries;
	e_state				_state;

public:
	/* Constructor */
	Entries();

	/* Methods */
	void	pushStatus(Status status);
	void	popStatus();
	Status&	getStatus();

	e_state getQueueStatus() const;
};