#pragma once

#include "IEventsHandler.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Context.hpp"

namespace ue
{

class BaseState : public IEventsHandler
{
public:
    BaseState(Context& context, const std::string& name);
    ~BaseState() override;

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleDisconnected() override;
    void handleSMS(common::PhoneNumber from, std::string text, common::MessageId msgType) override;
    void handleCallRequest(common::PhoneNumber) override;
    void handleCallAccepted(common::PhoneNumber) override;
    void handleCallDropped(common::PhoneNumber) override;
    void handleUnknownRecipientAfterCallRequest() override;
    void handleUnknownRecipientAfterCallAccepted() override;

protected:
    Context& context;
    common::PrefixedLogger logger;
};

}
