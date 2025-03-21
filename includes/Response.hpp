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

#include "EntryObserver.hpp"
#include "Entries.hpp"

class ResponseState;
class ContentHandler;
class Request;
class Entries;
class Entry;


class Response : public EntryObserver {
private:
	Request&							_request;
	Entries*							_entries; 
	ResponseState*						_state;
    std::map<int, std::string>			_serverMap;

	std::map<std::string, std::string>  _headers;
	std::string                         _body;

public:
	Response(Request& request, Entries* entries);		/* with params */
	~Response();

	// Response		to_response();
	
	/* ... */
	void			onEntryChanged();
	void			updateState();

	// /* to_string*/
	// std::string		to_string();

	// /* setter */
	// void			setBody(const std::string& content);

	// /* getter */
	// std::string							getBody() const;
	// std::string							getStatus() const;
	// std::map<std::string, std::string>	getHeaders() const;

	// /* add methods */
	// void		addHeader(const std::string& key, const std::string& value);
private:
	void		_cleanState();
	void		_replaceState(Entry::e_classes statusClass);
	bool		_shouldReuseState(Entry::e_classes statusClass) const;
	void		_prepareForEvaluation();
};

#endif 