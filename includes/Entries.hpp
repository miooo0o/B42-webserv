#pragma once

#include <deque>

class Status {
	/**
	 * @brief Status validation flags for the Entries class.
	 */
	enum e_validate {
		STATUS_PENDING         	= 0,	/* Default state, pending validation */
		STATUS_VALIDATED       	= 1,	/* Status code is valid and registered */
		_STATUS_UNKNOWN        	= -1,	/* Code is within valid range but not registered */
		_STATUS_OUT_OF_RANGE   	= -2,	/* Code is outside the HTTP status code range (100-599) */
		_STATUS_NOT				= -3	/* Code is not a valid HTTP status (e.g., negative or too large) */
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

	enum e_reference {
		REF_STATIC_MAP,
		REF_SERVER_CONFIG
	};

private:
    int			_code;
    e_validate	_valStatus;
    e_classes	_class;
    bool		_exposed;
	e_reference	_ref;

public:
    /* Constructor */
    Status();
    Status(int code);

    /* Getter methods */
    int			getCode() const;
    e_validate	getValStatus() const;
    e_classes	getClass() const;
    bool		isValidated() const;
	bool		isExposed() const;

    /* Validation methods ( this.method() ) */
	void		eval();
	bool		isInRange() const;
    bool		isInClassRange() const;

private:
    /* Validation methods */
    e_validate	_validate();
    e_classes	_classify();
};


class Entries {
public:
	/**
	 * @brief Queue status flags to track the current state of the status queue.
	 */
	enum e_state {
		_QUEUE_OVERFLOW		= -3,	/* The queue has exceeded its maximum capacity */
		_QUEUE_UNDERFLOW	= -2,	/* An attempt was made to pop from an empty queue */
		_QUEUE_ERROR		= -1,	/* An error occurred during status processing */
		QUEUE_EMPTY			= 0,	/* The queue is empty */
		QUEUE_PENDING,				/* There are statuses waiting to be processed */
		QUEUE_PROCESSING,			/* A status is currently being processed */
		QUEUE_COMPLETE,				/* The processing of a status is completed */
		QUEUE_FULL					/* The queue is full and cannot accept more statuses */
	};

private:
	std::deque<Status>	_entries;
	e_state				_state;
	bool				_readFromRequest;
public:
	/* Constructor */
	Entries(Request& request);
	~Entries();

	/* Methods */
	void	replace(Status status);
	void	push_back(Status status);
	void	pop_back();
	Status	get_back();

	e_state getQueueStatus() const;

	/* setter */
	void	setState(e_state state);

private:
	void	_initEntry(Request& request);
};