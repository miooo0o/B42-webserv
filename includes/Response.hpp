/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 23:27:22 by minakim           #+#    #+#             */
/*   Updated: 2025/03/20 23:27:23 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <string>
#include <map>

#include <iostream>

# include "StatusManager.hpp"
# include "EntryObserver.hpp"
# include "StatusEntry.hpp"

class ResponseState;
class ContentHandler;
class Request;
class StatusManager;
class StatusEntry;

class Response : public EntryObserver {
private:
	Request&							_request;
	StatusManager						_manager;
	ResponseState*						_state;
    std::map<int, std::string>			_serverMap;

	std::map<std::string, std::string>  _headers;
	std::string                         _body;

public:
	Response(Request& request);		/* with params */
	~Response();

	
	/* ... */
	void			addStatusCode(int code);
	StatusManager	getStatusManager() const;

private:
	void		_onEntryChanged();
	void		_syncState();
	bool		_handleFlowUpdate(StatusEntry& target);
	void		_handleStateReuse(StatusEntry& target);
	void		_cleanState();
	void		_assignNewState(StatusEntry::e_classes statusClass);
	bool		_shouldReuseState(StatusEntry::e_classes statusClass) const;
	void		_handleUpdateException(const std::exception& e);
	void		_syncCurrentEntry();
};

#endif 