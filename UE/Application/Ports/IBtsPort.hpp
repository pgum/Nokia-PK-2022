#pragma once

#include "Messages/BtsId.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue {

    class IBtsEventsHandler {
    public:
        virtual ~IBtsEventsHandler() = default;

        virtual void handleSib(common::BtsId) = 0;

        virtual void handleAttachAccept() = 0;

        virtual void handleAttachReject() = 0;

        virtual void handleDisconnected() = 0;

        virtual void handleSmsReceive(uint8_t, const std::string &, common::PhoneNumber, common::PhoneNumber) = 0;

        virtual void handleFailedSmsSend() = 0;

        virtual void handleCallRequest(common::PhoneNumber) = 0;

        virtual void handleCallDrop(common::PhoneNumber) = 0;

        virtual void handleCallAccept(common::PhoneNumber) = 0;

        virtual void handleCallTalk(common::PhoneNumber, std::string) = 0;

        virtual void handleUnknownRecipientCallRequest(common::PhoneNumber) = 0;
        virtual void handleUnknownRecipientCallTalk(common::PhoneNumber) = 0;

    };

    class IBtsPort {
    public:
        virtual ~IBtsPort() = default;

        virtual void sendAttachRequest(common::BtsId) = 0;

        virtual common::PhoneNumber getOwnPhoneNumber() = 0;

        virtual void sendSms(common::PhoneNumber, std::string) = 0;

        virtual void sendCallRequest(common::PhoneNumber) = 0;

        virtual void sendCallAccept(common::PhoneNumber) = 0;

        virtual void sendCallDrop(common::PhoneNumber) = 0;

        virtual void sendCallTalk(common::PhoneNumber, std::string) = 0;
    };

}
