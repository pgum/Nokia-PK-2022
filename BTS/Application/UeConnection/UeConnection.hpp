#pragma once

#include "IUeConnection.hpp"
#include "ITransport.hpp"
#include "UeRelay/IUeRelay.hpp"
#include "Synchronization.hpp"
#include "Logger/ILogger.hpp"

#include "Messages/MessageHeader.hpp"
#include "Messages/IncomingMessage.hpp"
#include "Logger/PrefixedLogger.hpp"

namespace bts
{
using common::MessageHeader;

class UeConnection : public IUeConnection
{
public:
    UeConnection(ITransportPtr transport, common::ILogger& logger, SyncGuardPtr syncGuard);
    ~UeConnection() override;

    void start(UeSlot ueSlot) override;

    void sendMessage(BinaryMessage message) override;
    void sendSib(BtsId btsId) override;
    PhoneNumber getPhoneNumber() const override;
    bool isAttached() const override;

    void print(std::ostream& os) const override;
private:

    void onUeMessageCallback(BinaryMessage message);
    void onUeMessageCallbackBody(BinaryMessage message);
    void onAttachRequest(PhoneNumber phoneNumber);
    bool forwardMessage(BinaryMessage message, PhoneNumber to);

    void onUeDisconnectedCallback();
    void stop();

    void sendAttachResponse(bool success, PhoneNumber phoneNumber);
    void sendUnknownRecipient(const MessageHeader& messageHeader);
    void sendUnknownSender(const MessageHeader& messageHeader);

    void attach(PhoneNumber phoneNumber);
    void detach();

    void printPrefix(std::ostream&);

    SyncGuardPtr syncGuard;
    UeSlot ueSlot;
    common::PrefixedLogger logger;
    ITransportPtr transport;
};

}
