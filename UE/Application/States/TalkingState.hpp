#pragma once

#include "BaseState.hpp"
#include "Messages/PhoneNumber.hpp"
#include "ConnectedState.hpp"

namespace ue {
    class TalkingState : public ConnectedState {
    private:
        common::PhoneNumber caller;
    public:
        TalkingState(Context &context, common::PhoneNumber caller);

// IBtsEventsHandler interface
        void handleUnknownRecipientCallTalk(common::PhoneNumber phoneNumber) override;

        void handleCallTalk(common::PhoneNumber phoneNumber, std::string message) override;
        void handleSendCallTalk(common::PhoneNumber phoneNumber, std::string message) override;
        void handleCallDrop(common::PhoneNumber) override;
        void handleSendCallDrop(common::PhoneNumber) override;
        void handleCallRequest(common::PhoneNumber) override;

        void handleSmsReceive(uint8_t action, const std::string &text, common::PhoneNumber fromPhoneNumber,
                              common::PhoneNumber toPhoneNumber) override;

        void handleTimeout() override;
        void handleDisconnected() override;

    };

}