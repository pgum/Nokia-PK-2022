#pragma once

#include "BaseState.hpp"
#include "ConnectedState.hpp"
#include "UeGui/ISmsComposeMode.hpp"

namespace ue {

    class SendingSmsState : public ConnectedState {
    public:
        SendingSmsState(Context &context);
        IUeGui::ISmsComposeMode& smsComposeMode;

    protected:

        void onAcceptCallbackClicked();

        void onDeclineCallbackClicked();

        void sendSms();
    };
}
