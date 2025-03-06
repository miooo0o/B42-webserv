#include "ResponseState.hpp"

InformationalState::InformationalState(int code) : ResponseState(code) {}

bool    InformationalState::isWithinRange()
{
    if (100 <= _code && 200 > _code && _statusFlag == STATUS_OK ) {
        return (true);
    }
    return (false);
}