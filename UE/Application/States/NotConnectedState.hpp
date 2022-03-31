#pragma once

#include "BaseState.hpp"

namespace ue
{

class NotConnectedState : public BaseState
{
public:
    NotConnectedState(Context& context);

    // IBtsEventsHandler interface
public:
    NotConnectedState(Context& context);
    virtual void handleSib(common::BtsId btsId) override;
};

}
