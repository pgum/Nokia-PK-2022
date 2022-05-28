#pragma once

#include "BaseState.hpp"
#include "ConnectedState.hpp"
#include "UeGui/IDialMode.hpp"

namespace ue {

    class SendingCallRequestState : public BaseState {
    public:
        SendingCallRequestState(Context &context);
        IUeGui::IDialMode &dialMode;

    protected:

        void onAcceptCallbackClicked();

        void onDeclineCallbackClicked();

        void makeCall();

        void handleDropCall(const common::PhoneNumber callerNumber);

        void makeDropCall(const common::PhoneNumber callerNumber);

    };
}

