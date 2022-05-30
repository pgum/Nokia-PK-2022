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
    void handleDisconnected() override;
    void handleSmsReceive(uint8_t action, const std::string& text, common::PhoneNumber fromPhoneNumber, common::PhoneNumber toPhoneNumber) override;
    void handleFailedSmsSend();
};

}
