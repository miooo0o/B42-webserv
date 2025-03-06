/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HtmlHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 14:54:59 by minakim           #+#    #+#             */
/*   Updated: 2025/03/06 15:40:32 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include "ResponseState.hpp"
#include "Utils.hpp"

class ResponseState;
/* static class to handle `.html` files */
class HtmlHandler {
public:
    static std::string  createHtml(ResponseState& state);
    // static std::string  createHtml(ResponseState& state /*, server config _map for error pages*/);

};

/* ************************************************************************** */

std::string HtmlHandler::createHtml(ResponseState& state)
{
    if (state.isWithinRange()) {
        if (!state.isExposed()) {
            // If status is informational, skip HTML file creation and
            // return: “This action should not occur.”
        }
    }
    return ( /* error */ );
}
