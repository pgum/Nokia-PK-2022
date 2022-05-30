#pragma once

#include "Messages/BtsId.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Messages.hpp"
namespace ue
{

class IBtsEventsHandler
{
public:
    virtual ~IBtsEventsHandler() = default;

    virtual void handleSib(common::BtsId) = 0;
    virtual void handleAttachAccept() = 0;
    virtual void handleAttachReject() = 0;
    virtual void handleDisconnected() = 0;
    virtual void handleSMS(common::PhoneNumber from, std::string text, common::MessageId msgType) = 0;
    virtual void handleCallRequest(common::PhoneNumber from) = 0;
    virtual void handleCallAccepted(common::PhoneNumber from) = 0;
    virtual void handleCallDropped(common::PhoneNumber from) = 0;
    virtual void handleUnknownRecipientAfterCallRequest() = 0;
    virtual void handleUnknownRecipientAfterCallAccepted() = 0;

};

class IBtsPort
{
public:
    virtual ~IBtsPort() = default;

    virtual void sendAttachRequest(common::BtsId) = 0;
    virtual void sendSMS(common::PhoneNumber to, std::string msg) = 0;
    virtual void sendCallRequest(common::PhoneNumber to) = 0;
    virtual void sendCallAccepted(common::PhoneNumber to) = 0;
    virtual void sendCallDropped(common::PhoneNumber to) = 0;

};

}
