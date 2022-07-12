#pragma once

#include "IEventsHandler.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Context.hpp"

namespace ue {

    class BaseState : public IEventsHandler {
    public:
        BaseState(Context &context, const std::string &name);

        ~BaseState() override;

        // ITimerEventsHandler interface
        void handleTimeout() override;

        // IBtsEventsHandler interface
        void handleSib(common::BtsId btsId) override;

        void handleAttachAccept() override;

        void handleAttachReject() override;

        void handleDisconnected() override;

        void handleSmsReceive(uint8_t action, const std::string &text, common::PhoneNumber fromPhoneNumber,
                              common::PhoneNumber toPhoneNumber) override;

        void handleFailedSmsSend() override;

        void handleCallRequest(common::PhoneNumber phoneNumber) override;

        void handleUnknownRecipientCallRequest(common::PhoneNumber) override;

        void handleUnknownRecipientCallTalk(common::PhoneNumber) override;

        void handleCallAccept(common::PhoneNumber receiverPhoneNumber) override;

        void handleCallDrop(common::PhoneNumber receiverPhoneNumber) override;

        void handleCallTalk(common::PhoneNumber, std::string) override;

        //IUserEventsHandler interface:
        void handleSendCallAccept(common::PhoneNumber) override;

        void handleStartDial() override;

        void handleSendCallRequest(common::PhoneNumber) override;
//
        void handleSendCallDrop(common::PhoneNumber) override;

        void handleSendCallTalk(common::PhoneNumber, std::string) override;




    protected:
        Context &context;
        common::PrefixedLogger logger;
    };

}
