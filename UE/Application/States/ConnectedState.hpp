#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);

    virtual void showSmsButton();
    virtual void closeSmsButton();
    void handleSmsReceive(uint8_t action, std::string text) override;
    void handleDisconnected() override;
};

}
