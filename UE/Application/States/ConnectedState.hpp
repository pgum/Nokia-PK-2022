#pragma once

#include "BaseState.hpp"

namespace ue
{



    class ConnectedState : public BaseState
    {
    public:
        ConnectedState(Context& context);
        void handleDisconnected() override;
        void handleSMSReceive(const std::string smsText, const common::PhoneNumber senderNumber) override;
    protected:
        void onAcceptCallbackClicked();
        void onDeclineCallbackClicked();
    };



}