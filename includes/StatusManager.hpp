/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusManager.hpp                                        :+:      :+:    :+:   */
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
#include "Error.hpp"

#include "EntryObserver.hpp"
#include "StatusQueue.hpp"

class Request;

class StatusManager {
private:
	const Request*				_request;
	StatusQueue					_statusQueue;
	std::vector<EntryObserver*>	_observers;
	Error						_error;

public:
	/* Constructor */
	StatusManager(const Request* request);
	StatusManager(int statusCode);

	/* Copy */
    StatusManager(const StatusManager& other);
	/* Destructor */
    ~StatusManager();

	/* EntryObserver */
	void	addObserver(EntryObserver* observer);
	void	removeObserver(EntryObserver* observer);
	void	archiveAll();
	void	reuseEntry(int newCode);

	/* Methods */
	bool	ready();
	bool	eval(const std::map<int, std::string>* serverScenarios);

	void	replace(StatusEntry status);
	void	resetEntryWith(StatusEntry& newEntry);
	void	push(StatusEntry status);
	void	pop();

	StatusQueue&		getStatusQueue();
	const StatusQueue&	getStatusQueue() const;

private:
	/* Initialization */
	void	_initEntry();
	void    _initEntry(int statusCode);

	/* EntryObserver */
	void	_notifyObservers();


	/* Validation methods */
    void	_validateCodeRange();
	void	_findResponseSource();
	void	_fallbackToInternalError(StatusEntry& entry);

	// TODO(@mina): delete this method, it will replace to _validateWithSources() (-ing)
	bool	_validateWithMap(const std::map<int, std::string>* refMap, StatusEntry::e_source refType);

	bool	_validateWithSources();
	bool	_returnSource(StatusEntry& entry, StatusEntry::e_source src );
	void	_autoSetClasses(StatusEntry& entry);
	bool	_isReadyToClassify(StatusEntry& entry);

};

#endif