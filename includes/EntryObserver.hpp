#pragma once

class EntryObserver {
public:
    virtual			~EntryObserver() {}
    virtual void	onEntryChanged() = 0;
};
    