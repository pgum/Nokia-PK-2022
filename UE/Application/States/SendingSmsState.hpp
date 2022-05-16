#pragma once

#include "BaseState.hpp"
#include "ConnectedState.hpp"
#include "UeGui/ISmsComposeMode.hpp"

namespace ue {

    class SendingSmsState : public ConnectedState {
    public:
        SendingSmsState(Context &context, int powiadomienie);

        IUeGui::ISmsComposeMode &smsComposeMode;
        int notification = 0;

    protected:

        void onAcceptCallbackClicked();

        void onDeclineCallbackClicked();

        void sendSms();
    };
}
