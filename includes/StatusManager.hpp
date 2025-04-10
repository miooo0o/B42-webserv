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
#include <vector>

/* related haders */
#include "EntryObserver.hpp"
#include "StatusQueue.hpp"

class Request;

class StatusManager {
public:

private:
	StatusQueue					_statusQueue;
	std::vector<EntryObserver*>	_observers;

public:
	/* Constructor */
	StatusManager(const Request& request);
	/* */
    StatusManager(const StatusManager& other);
	/* */
    ~StatusManager();

	/* EntryObserver */
	void	addObserver(EntryObserver* observer);
	void	removeObserver(EntryObserver* observer);
	void	archiveAll();
	void	reuseEntry(int newCode);

	/* Methods */
	bool	ready();
	bool	eval(const std::map<int, std::string>& serverScenarios);

	void	replace(StatusEntry status);
	void	resetEntryWith(StatusEntry& newEntry);
	void	push(StatusEntry status);
	void	pop();

	StatusQueue&		getStatusQueue();
	const StatusQueue&	getStatusQueue() const;

private:
	/* Initialization */
	void	_initEntry(const Request& request);

	/* EntryObserver */
	void	_notifyObservers();


	/* Validation methods */
    void	_validateCodeRange();
	void	_findCodeReference(const std::map<int, std::string>& serverSideMaps);
	void	_fallbackToInternalError(StatusEntry& entry);
	bool	_validateWithMap(const std::map<int, std::string>& refMap, StatusEntry::e_reference refType);
	void	_autoSetClasses(StatusEntry& entry);
	bool	_isReadyToClassify(StatusEntry& entry);

};

#endif