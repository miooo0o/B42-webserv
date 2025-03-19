#pragma once

#include <deque>

/* Define MAX_LOG_QUEUE_SIZE with an appropriate value */
#define MAX_LOG_QUEUE_SIZE 20

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
	e_reference     _ref;           /* Source of Map scenarios */

public:
    /* Constructor */
    Entry();
    Entry(int code);

    /* Getter methods */
    int			getCode() const;
    e_entry_val	getValidateStatus() const;
    e_classes	getClass() const;
	bool		isExposed() const;
    bool		isReady() const;
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
 * The Entries class ensures that only one status entry is processed at a time.
 * It validates and classifies status codes while maintaining a log of past statuses.
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
	std::deque<Entry>		_entries;   		/* Holds current entries */
	std::vector<EntryObserver*>	_observers;
	e_que_entries			_state;     		/* Tracks queue status */
	bool                	_readFromRequest;	/* Indicates if request data was read */
	std::deque<Entry>   	_log;       		/* Keeps a record of past entries */
public:
	/* Constructor */
	Entries(Request& request);
	~Entries();

	/* EntryObserver */
	void	addObserver(EntryObserver* observer);

	/* Methods */
	bool	ready();
	bool	eval(const std::map<int, std::string>& serverScenarios);
	
	void	replace(Entry status);
	void	push_back(Entry status);
	void	pop_front();

	/* Get _entries.front() */
	Entry	getEntry();
	Entry	getEntry() const;
	
	/* Queue status management */
	
	e_que_entries getQueueStatus() const;
	void	setState(e_que_entries state);
	
	/**
	 * // TODO: implement methods, refactor
	 */
	bool				hasSingleEntry() const;
	bool				isExposed() const;
	bool				isInRange() const;
	bool				isInClassRange() const;
	bool				isValidated() const;
	bool				isPending() const;

	int					getCode() const;
	Entry::e_entry_val	getStatus() const;
	Entry::e_classes	getClass() const;
	Entry::e_reference	getMapRef() const;

	void				setCode(int code);
	void				setStatus(Entry::e_entry_val status);
	void				setClass(Entry::e_classes entryClass);
	void				setExposed(bool res);
	void				setMapRef(Entry::e_reference ref);

private:
	/* Initialization */
	void	_initEntry(Request& request);

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
#define QUEUE_HAS_ERROR		( _state < QUEUE_EMPTY ) 
#define QUEUE_HAS_DATA		( _state > QUEUE_EMPTY )
#define QUEUE_IS_ACTIVE     ( _state >= QUEUE_PENDING ) 