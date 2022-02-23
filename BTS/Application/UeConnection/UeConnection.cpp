#include "UeConnection.hpp"
#include "Messages/IncomingMessage.hpp"
#include "Messages/OutgoingMessage.hpp"

namespace bts
{

using namespace std::placeholders;
using common::MessageId;

UeConnection::UeConnection(ITransportPtr transport, common::ILogger &logger, SyncGuardPtr syncGuard)
    : syncGuard(syncGuard),
      transport(transport),
      logger(logger, std::bind(&UeConnection::printPrefix, this, _1))
{
}

UeConnection::~UeConnection()
{
    stop();
}

void UeConnection::start(UeSlot ueSlot)
{
    this->ueSlot = ueSlot;
    transport->registerDisconnectedCallback(std::bind(&UeConnection::onUeDisconnectedCallback, this));
    transport->registerMessageCallback(std::bind(&UeConnection::onUeMessageCallback, this, _1));
}

void UeConnection::stop()
{
    transport->registerMessageCallback(nullptr);
    transport->registerDisconnectedCallback(nullptr);
}

void UeConnection::sendAttachResponse(bool success, PhoneNumber phoneNumber)
{
    common::OutgoingMessage messageBuilder(MessageId::AttachResponse, PhoneNumber{}, phoneNumber);
    messageBuilder.writeNumber<bool>(success);
    sendMessage(messageBuilder.getMessage());
}

void UeConnection::sendSib(BtsId btsId)
{
    common::OutgoingMessage messageBuilder(MessageId::Sib, PhoneNumber{}, PhoneNumber{});
    messageBuilder.writeBtsId(btsId);
    sendMessage(messageBuilder.getMessage());
}

PhoneNumber UeConnection::getPhoneNumber() const
{
    return ueSlot.getPhoneNumber();
}

void UeConnection::sendMessage(BinaryMessage messageToSend)
{
    transport->sendMessage(std::move(messageToSend));
}

void UeConnection::sendUnknownRecipient(const MessageHeader &messageHeader)
{
    common::OutgoingMessage messageBuilder(MessageId::UnknownRecipient, PhoneNumber{}, getPhoneNumber());
    messageBuilder.writeMessageHeader(messageHeader);
    sendMessage(messageBuilder.getMessage());
}

void UeConnection::sendUnknownSender(const MessageHeader &messageHeader)
{
    common::OutgoingMessage messageBuilder(MessageId::UnknownSender, PhoneNumber{}, getPhoneNumber());
    messageBuilder.writeMessageHeader(messageHeader);
    sendMessage(messageBuilder.getMessage());
}

void UeConnection::attach(PhoneNumber phoneNumber)
{
    ueSlot.attach(phoneNumber);
}

void UeConnection::detach()
{
    // that is probably last operation on this object!
    ueSlot.remove();
}

bool UeConnection::isAttached() const
{
    return ueSlot.isAttached();
}

void UeConnection::onUeMessageCallbackBody(BinaryMessage message)
{
    common::IncomingMessage incomingMessage(message);
    MessageHeader messageHeader = incomingMessage.readMessageHeader();

    if (messageHeader.messageId == MessageId::AttachRequest)
    {
        onAttachRequest(messageHeader.from);
    }
    else
    {
        if (not isAttached() or getPhoneNumber() != messageHeader.from)
        {
            logger.logError("Not ready for: ", messageHeader);
            sendUnknownSender(messageHeader);
        }
        else if (not forwardMessage(std::move(message), messageHeader.to))
        {
            logger.logError("Cannot forward: ", messageHeader);
            sendUnknownRecipient(messageHeader);
        }
        else
        {
            logger.logDebug("Forwarded: ", messageHeader);
        }
    }
}

void UeConnection::onUeMessageCallback(BinaryMessage message)
{
    SyncLock lock(*syncGuard);
    try
    {
        onUeMessageCallbackBody(message);
    }
    catch (std::exception& ex)
    {
        logger.logError("Ue message handling error: ", ex.what());
    }
}

void UeConnection::onAttachRequest(PhoneNumber phoneNumber)
{
    if (phoneNumber == PhoneNumber{})
    {
        // special case #1
        logger.logError("Attach with UE number equal to zero rejected");
        sendAttachResponse(false, phoneNumber);
        return;
    }

    if (isAttached())
    {
        if ( this->getPhoneNumber() == phoneNumber)
        {
            // special case #2
            logger.logError("Attach to UE already attached with identical number accepted");
            sendAttachResponse(true, phoneNumber);
            return;
        }
        // special case #3
        logger.logError("ReAttach with other number: ", phoneNumber);
        // no we - as, nevertheless, we want to attach
    }
    attach(phoneNumber);

    if (not isAttached())
    {
        logger.logError("Attach failed for number: ", phoneNumber);
        sendAttachResponse(false, phoneNumber);
        return;
    }

    logger.logInfo("Attached");
    sendAttachResponse(true, phoneNumber);
}

bool UeConnection::forwardMessage(BinaryMessage message, PhoneNumber to)
{
    return ueSlot.sendMessage(std::move(message), to);
}

void UeConnection::onUeDisconnectedCallback()
{
    SyncLock lock(*syncGuard);

    try
    {
        logger.logInfo("Disconnected");
        detach();
    }
    catch (std::exception& ex)
    {
        logger.logError("Disconnected error: ", ex.what());
    }
}

void UeConnection::printPrefix(std::ostream &os)
{
    os << "[UE:" << *this << "]";
}

void UeConnection::print(std::ostream &os) const
{
    os << transport->addressToString()
       << ":" << getPhoneNumber()
       << ":" << (isAttached() ? "A" : "I");
}

}
