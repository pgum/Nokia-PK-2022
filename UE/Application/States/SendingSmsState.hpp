#pragma once

#include "BaseState.hpp"
#include "ConnectedState.hpp"
#include "UeGui/ISmsComposeMode.hpp"

namespace ue {

    class SendingSmsState : public BaseState {
    public:
        SendingSmsState(Context &context, int notification);
        IUeGui::ISmsComposeMode &smsComposeMode;
        int notification = 0;

    protected:
        void onAcceptCallbackClicked();
        void onDeclineCallbackClicked();
        void handleSMSReceive(const std::string smsText, const common::PhoneNumber senderNumber);
        void sendSms();
    };
}