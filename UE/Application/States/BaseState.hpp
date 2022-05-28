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
        void handleDisconnected() override;
        void handleSib(common::BtsId btsId) override;
        void handleAttachAccept() override;
        void handleAttachReject() override;
        void handleSMSReceive(const std::string smsText, const common::PhoneNumber senderNumber) override;
        void handleCallRequest(const common::PhoneNumber callerNumber) override;
        void handleDropCall(const common::PhoneNumber callerNumber) override;
        void makeDropCall(const common::PhoneNumber callerNumber) override;

    protected:
        Context& context;
        common::PrefixedLogger logger;
    };

}
