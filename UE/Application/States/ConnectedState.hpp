#pragma once

#include "BaseState.hpp"

namespace ue
{



    class ConnectedState : public BaseState
    {
    public:
        ConnectedState(Context& context, int powiadomienie);
        void handleDisconnected() override;
        void handleSMSReceive(const std::string smsText, const common::PhoneNumber senderNumber) override;
        int notification = 0;
    protected:
        void onAcceptCallbackClicked();
        void onDeclineCallbackClicked();
    };



}