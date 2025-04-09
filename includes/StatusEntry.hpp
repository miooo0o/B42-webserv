#ifndef STATUSENTRY_HPP
# define STATUSENTRY_HPP

#include <ostream>

/**
 * @class StatusEntry
 * @brief Represents a single HTTP status entry.
 *
 * The StatusEntry class encapsulates HTTP status codes, their validation status,
 * classification, and reference type.
 */
class StatusEntry {
public:
	/**
     * @enum e_range
     * @brief Represents validation states of an Entry.
     */
	enum e_range {
		RANGE_PENDING       = 0,	/* Default state, pending validation */
		RANGE_VALIDATED     = 1,	/* Entry code is valid and registered */
		_RANGE_UNKNOWN      = -1,	/* Code is within valid range but not registered */
		_OUT_OF_RANGE   	= -2,	/* Code is outside the HTTP status code range (100-599) */
		_NOT_STATUS_CODE	= -3	/* Code is not a valid HTTP status (e.g., negative or too large) */
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
	 *
	 */
	enum e_flow {
		FLOW_UNSET = 0,			/* default / created */
		FLOW_READY,				/* passed eval */
		FLOW_PENDING_REUSE,		/*  if update need */
		FLOW_PROCESSED			/*  processed */
	};

    /**
     * @enum e_source
     * @brief Indicates where the status code was found.
     */
    enum e_source {
        SRC_ERROR_CLASS,				/* Found in predefined static map */
        SRC_SERVER_CONFIG,			/* Found in server config */
    	SRC_STATIC_RESPONSE,			/* if static response is called */
        _SRC_FALLBACK_INTERNAL      /* if Maps search attempt fail multi time */
    };

	/**
	 */

	enum e_error {
		NO_ERROR = 0,
		ERROR_EVALUATION
	};
private:
	int         _code;          /* HTTP status code */
	e_range		_range;			/* Validation status */
	e_classes   _class;         /* Classification */
	bool        _exposed;       /* Determines if status should be exposed as .html format */
	e_source	_errorSource;  /* Source of Map scenarios */
	e_flow		_flow;			/* */
	e_error		_error;

public:
    /* Constructor */
    StatusEntry();
    StatusEntry(int code);


	static bool	same_class(const StatusEntry& a, int code) {
		return (a.getClass() == static_cast<e_classes>(code / 100));
	}

    /* Getter */
    int			getCode() const;
    e_range		getRange() const;
    e_classes	getClass() const;
	e_source	getErrorSource() const;
	e_flow		getFlow() const;
	e_error		getError() const;

	/* setter */
	void		setCode(int code);
    void		setRange(e_range status);
    void		setClass(e_classes entryClass);
	void		setExposed(bool res);
	void		setErrorSource(e_source ref);
	void		setFlow(e_flow f);
	void		setError(e_error e);

    /* Validation methods ( this.method() ) */
	bool		isExposed() const;
	bool		isInRange() const;
    bool		isInClassRange() const;
};


#endif //STATUSENTRY_HPP

std::ostream& operator<<(std::ostream& os, const StatusEntry& entry);