#pragma once

#include "Logger/PrefixedLogger.hpp"
#include "Messages/PhoneNumber.hpp"
#include "IEventsHandler.hpp"
#include "Context.hpp"

namespace ue
{

    using common::PhoneNumber;
    using common::ILogger;

    class Application : public IEventsHandler
    {
    public:
        Application(PhoneNumber phoneNumber,
                    ILogger& iLogger,
                    IBtsPort& bts,
                    IUserPort& user,
                    ITimerPort& timer);
        ~Application();

        // ITimerEventsHandler interface
        void handleTimeout() override;

        // IBtsEventsHandler interface
        void handleSib(common::BtsId btsId) override;
        void handleAttachAccept() override;
        void handleAttachReject() override;
        void handleDisconnected() override;
        void handleSMSReceive(const std::string smsText, const common::PhoneNumber senderNumber) override;
        void handleCallRequest(const common::PhoneNumber callerNumber) override;
        void handleDropCall(const common::PhoneNumber callerNumber) override;
        void makeDropCall(const common::PhoneNumber callerNumber) override;

    private:
        Context context;
        common::PrefixedLogger logger;

    };

}
