#pragma once

#include "BaseState.hpp"

namespace ue {

    class ConnectedState : public BaseState {
    public:
        ConnectedState(Context &context, int notification);

        void handleDisconnected() override;

        void handleSMSReceive(const std::string smsText, const common::PhoneNumber senderNumber) override;

        void handleCallRequest(const common::PhoneNumber callerNumber) override;

        int notification = 0;
    protected:
        void onAcceptCallbackClicked();

        void onDeclineCallbackClicked();
    };

}