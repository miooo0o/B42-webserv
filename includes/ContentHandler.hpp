/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ContentHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 14:54:59 by minakim           #+#    #+#             */
/*   Updated: 2025/03/10 20:12:58 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

#include "Utils.hpp"

class ResponseState;
class ContentHandler {
private:
	// ServerConfig&					_serverConfig;
public:
	ContentHandler();
	// ContentHandler( /* server config & */ )
	~ContentHandler();

	std::string buildResponseBody(const ResponseState* state) const;
private:
	std::string	_filePath;
	bool		_useFile;
};

/* ************************************************************************** */

ContentHandler::ContentHandler() {}

ContentHandler::~ContentHandler() {}

std::string	ContentHandler::buildResponseBody(const ResponseState* state) const {
}
