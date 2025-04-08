/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EntryObserver.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 23:27:25 by minakim           #+#    #+#             */
/*   Updated: 2025/03/20 23:27:50 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENTRTOBSERVER_HPP
# define ENTRTOBSERVER_HPP

/**
 * @brief This class is used as an interface/basic
 *        for the Response class.
 */
class EntryObserver {
public:
    virtual			~EntryObserver() {}

    /**
     * @brief `_onEntryChanged` method is implemented
     *         in the Response class.
     */
    virtual void	_onEntryChanged() = 0;
};

#endif