/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Entries.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 23:28:00 by minakim           #+#    #+#             */
/*   Updated: 2025/03/20 23:32:06 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENTRIES_HPP
# define ENTRIES_HPP

/* headers */
#include <deque>
#include <map>
#include <iostream>

/* related haders */
#include "EntryObserver.hpp" 

/* Define MAX_LOG_QUEUE_SIZE with an appropriate value */
#define MAX_LOG_QUEUE_SIZE 20

class Request;

/**
 * @class Entry
 * @brief Represents a single HTTP status entry.
 * 
 * The Entry class encapsulates HTTP status codes, their validation status, 
 * classification, and reference type.
 */
class Entry {
public:
	/**
     * @enum e_entry_val
     * @brief Represents validation states of an Entry.
     */
	enum e_entry_val {
		ENTRY_PENDING         	= 0,	/* Default state, pending validation */
		ENTRY_VALIDATED       	= 1,	/* Entry code is valid and registered */
		_ENTRY_UNKNOWN        	= -1,	/* Code is within valid range but not registered */
		_ENTRY_OUT_OF_RANGE   	= -2,	/* Code is outside the HTTP status code range (100-599) */
		_ENTRY_NOT				= -3	/* Code is not a valid HTTP status (e.g., negative or too large) */
	};

    /** 
     * @enum e_classes
     * @brief Represents HTTP status code classification.
     */
	enum e_classes {
		_NOT_CLASSIFY			= 0,	/* Default unclassified state */
		INFORMATIONAL			= 1,	/* 1xx: Informational responses */
		SUCCESSFUL				= 2,	/* 2xx: Successful responses */
		REDIRECTION				= 3,	/* 3xx: Redirection messages */
		CLIENT_ERROR			= 4,	/* 4xx: Client error responses */
		SERVER_ERROR			= 5 	/* 5xx: Server error responses */
	};


    /**
     * @enum e_reference
     * @brief Indicates where the status code was found.
     */
    enum e_reference {
        REF_STATIC_MAP,     /* Found in predefined static map */
        REF_SERVER_CONFIG,  /* Found in server config */
        _500_NOT_FOUND      /* if Maps search attempt fail multiful time */
    };

private:
	int             _code;          /* HTTP status code */
	e_entry_val     _entryVal;      /* Validation status */
	e_classes       _class;         /* Classification */
	bool            _exposed;       /* Determines if status should be exposed as .html format */
	e_reference     _scenariosMap;  /* Source of Map scenarios */

public:
    /* Constructor */
    Entry();
    Entry(int code);

    /* Getter methods */
    int			getCode() const;
    e_entry_val	getValidateStatus() const;
    e_classes	getClass() const;
	bool		isExposed() const;
	e_reference	getMapReference() const;

	/* setter */
	void		setCode(int code);
    void		setValidateStatus(e_entry_val status);
    void		setClass(e_classes entryClass);
	void		setExposed(bool res);
	void		setMapReference(e_reference ref);

    /* Validation methods ( this.method() ) */
	bool		isInRange() const;
    bool		isInClassRange() const;
};

/**
 * @class Entries
 * @brief Manages a queue of HTTP status entries.
 * 
 * The Entries class is responsible for handling HTTP status entries in a controlled queue system.
 * It ensures that only one status entry is processed at a time, and previous statuses are logged
 * for reference. The queue enforces strict management rules:
 * 
 * - **Single Active Entry:** The queue must contain exactly one active status entry at all times.
 * - **Overflow Handling:** If multiple entries are unintentionally pushed, the system flags an 
 *   overflow error (`_QUEUE_OVERFLOW`).
 * - **Logging Mechanism:** Processed entries are stored in `_log`, allowing debugging and history tracking.
 * - **Validation & Classification:** Entries are validated and classified upon addition to the queue.
 * - **Observer Pattern:** The class supports observers to monitor queue state changes, triggering 
 *   state updates in the Response class.
 * 
 * #### Queue States:
 * - `QUEUE_EMPTY`: No active status, waiting for a new request.
 * - `QUEUE_FULL`: A single valid status entry exists, ready for evaluation.
 * - `QUEUE_PENDING`: The entry has been validated and is ready for processing.
 * - `QUEUE_PROCESSING`: The response system is actively handling the status.
 * - `QUEUE_COMPLETE`: The entry has been fully processed.
 * - `_QUEUE_ERROR`: An unexpected issue occurred in entry processing.
 * 
 * #### Method Responsibilities:
 * - `push_back(Entry status)`: Adds a new entry while ensuring only one active entry exists.
 * - `pop_front()`: Removes the current entry and checks if the queue is now empty.
 * - `replace(Entry status)`: Replaces the current entry with a new one.
 * - `eval(const std::map<int, std::string>& serverScenarios)`: Validates and classifies the entry.
 * - `_validateCodeRange()`: Ensures the code falls within HTTP standard ranges.
 * - `_findCodeReference()`: Determines if the code exists in predefined maps.
 * - `_notifyObservers()`: Notifies attached observers (e.g., `Response`) about changes.
 * 
 * #### Design Notes:
 * - **Deque Usage:** While `std::deque<Entry>` is used for `_entries`, it is always constrained to 
 *   exactly one active entry at a time. This choice is for fast insertion/removal rather than 
 *   traditional queue behavior.
 * - **Strict Status Control:** The `_state` variable tightly regulates queue behavior, ensuring 
 *   entries transition smoothly through processing stages.
 * - **Logging & Debugging:** A secondary `_log` queue retains past statuses for troubleshooting.
 * 
 * This structure enforces strict HTTP status handling while allowing flexible response processing.
 */
class Entries {
public:
    /**
     * @enum e_que_entries
     * @brief Represents the state of the queue.
     */
	enum e_que_entries {
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
	std::deque<Entry>			_entries;   		/* Holds current entries */
	std::vector<EntryObserver*>	_observers;			/* observer patten to control _entries queue's change */
	e_que_entries				_queueLevel;     	/* Tracks queue status */
	std::deque<Entry>   		_log;       		/* Keeps a record of past entries */
public:
	/* Constructor */
	Entries(const Request& request);
	Entries(const Entries& other);
	~Entries();

	/* EntryObserver */
	void	addObserver(EntryObserver* observer);
	void	removeObserver(EntryObserver* observer);
	/* Methods */
	bool	ready();
	bool	eval(const std::map<int, std::string>& serverScenarios);
	
	void	replace(Entry status);
	void	resetEntryWith(Entry& newEntry);
	void	push_back(Entry status);
	void	pop_front();
	
	
	bool				hasSingleEntry() const;
	bool				isExposed() const;
	bool				isInRange() const;
	bool				isInClassRange() const;
	bool				isValidated() const;
	bool				isPending() const;
	bool				isProcessing() const;
	
	/* get _entries.front() */
	Entry&				getEntry();
	Entry				getEntry() const;
	
	/* getter */
	e_que_entries		getQueueStatus() const; /* Queue status management */
	
	int					getCode() const;
	Entry::e_entry_val	getStatus() const;
	Entry::e_classes	getClass() const;
	Entry::e_reference	getMapRef() const;

	/* setter */
	void				setQueLevel(e_que_entries queueLevel); /* Queue status management */
	
	void				setCode(int code);
	void				setStatus(Entry::e_entry_val status);
	void				setClass(Entry::e_classes entryClass);
	void				setExposed(bool res);
	void				setMapRef(Entry::e_reference ref);
	
private:
	/* Initialization */
	void	_initEntry(const Request& request);

	/* EntryObserver */
	void	_notifyObservers();

	/* Logging */
	void	_recodeStatus();
	void	_recodeStatus(Entry& status);
	void	_enforceLogSizeLimit();


	/* Validation methods */
    void	_validateCodeRange();
	void	_findCodeReference(const std::map<int, std::string>& serverSideMaps);
	bool	_validateWithMap(const std::map<int, std::string>& refMap, Entry::e_reference refType);
	void	_classify();

};

/* Macros for queue state checks */
#define QUEUE_HAS_ERROR		( _queueLevel < QUEUE_EMPTY ) 
#define QUEUE_HAS_DATA		( _queueLevel > QUEUE_EMPTY )
#define QUEUE_IS_ACTIVE     ( _queueLevel >= QUEUE_PENDING )

#endif