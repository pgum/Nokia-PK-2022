#pragma once

#include "BaseState.hpp"

namespace ue {

    class ViewingSingleSmsState : public BaseState {
    public:
        ViewingSingleSmsState(Context &context, int smsListPosition);

    protected:

        void onAcceptCallbackClicked();

        void onDeclineCallbackClicked();

        void handleSMSReceive(const std::string smsText, const common::PhoneNumber senderNumber);
    };
}