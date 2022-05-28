#pragma once

#include "IBtsPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "ITransport.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

    class BtsPort : public IBtsPort
    {
    public:
        BtsPort(common::ILogger& logger, common::ITransport& transport, common::PhoneNumber phoneNumber);
        void start(IBtsEventsHandler& handler);
        void stop();

        void sendAttachRequest(common::BtsId) override;
        void sendSms(common::PhoneNumber receiverPhoneNumber, std::string smsText) override;
        void makeCall(common::PhoneNumber receiverPhoneNumber) override;
        void declineCall(common::PhoneNumber callerPhoneNumber) override;

    private:
        void handleMessage(BinaryMessage msg);

        common::PrefixedLogger logger;
        common::ITransport& transport;
        common::PhoneNumber phoneNumber;

        IBtsEventsHandler* handler = nullptr;
    };

}
