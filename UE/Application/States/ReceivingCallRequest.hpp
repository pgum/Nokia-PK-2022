#pragma once

#include "BaseState.hpp"
#include "UeGui/ITextMode.hpp"

namespace ue {

    class ReceivingCallRequest : public BaseState {
    public:
        ReceivingCallRequest(Context &context, const common::PhoneNumber callerNumber);
        IUeGui::ITextMode &alertMode;
        const common::PhoneNumber callerNumber;

    protected:

        void onAcceptCallbackClicked();

        void onDeclineCallbackClicked();

        void handleCallRequest(const common::PhoneNumber callerNumber);

        void makeDropCall(const common::PhoneNumber callerNumber);

        void handleDropCall(const common::PhoneNumber callerNumber);

    };
}
