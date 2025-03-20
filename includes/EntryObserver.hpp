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
class EntryObserver {
public:
    virtual			~EntryObserver() {}
    virtual void	onEntryChanged() = 0;
};

#endif