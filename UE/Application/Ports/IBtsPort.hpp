#pragma once

#include "Messages/BtsId.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

    class IBtsEventsHandler
    {
    public:
        virtual ~IBtsEventsHandler() = default;

        virtual void handleDisconnected() = 0;

        virtual void handleSib(common::BtsId) = 0;
        virtual void handleAttachAccept() = 0;
        virtual void handleAttachReject() = 0;
        virtual void handleSMSReceive(const std::string smsText, const common::PhoneNumber senderNumber) = 0;
        virtual void handleCallRequest(const common::PhoneNumber callerNumber) = 0;
        virtual void handleDropCall(const common::PhoneNumber callerNumber) = 0;
        virtual void makeDropCall(const common::PhoneNumber callerNumber) = 0;
    };

    class IBtsPort
    {
    public:
        virtual ~IBtsPort() = default;

        virtual void sendAttachRequest(common::BtsId) = 0;
        virtual void sendSms(common::PhoneNumber, std::string)=0;
        virtual void makeCall(common::PhoneNumber) = 0;
        virtual void declineCall(common::PhoneNumber) = 0;
    };

}
