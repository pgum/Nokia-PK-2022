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
    };

    class IBtsPort
    {
    public:
        virtual ~IBtsPort() = default;

        virtual void sendAttachRequest(common::BtsId) = 0;
        virtual void sendSms(common::PhoneNumber, std::string)=0;
    };

}