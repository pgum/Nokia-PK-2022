#include "BtsPort.hpp"
#include "Messages/IncomingMessage.hpp"
#include "Messages/OutgoingMessage.hpp"

namespace ue
{

BtsPort::BtsPort(common::ILogger &logger, common::ITransport &transport, common::PhoneNumber phoneNumber)
    : logger(logger, "[BTS-PORT]"),
      transport(transport),
      phoneNumber(phoneNumber)
{}

void BtsPort::start(IBtsEventsHandler &handler)
{
    transport.registerMessageCallback([this](BinaryMessage msg) {handleMessage(msg);});
    transport.registerDisconnectedCallback([this] { this->handler->handleDisconnected(); });
    this->handler = &handler;
}

void BtsPort::stop()
{
    transport.registerMessageCallback(nullptr);
    transport.registerDisconnectedCallback(nullptr);
    handler = nullptr;
}

void BtsPort::handleMessage(BinaryMessage msg)
{
    try
    {
        common::IncomingMessage reader{msg};
        auto msgId = reader.readMessageId();
        auto from = reader.readPhoneNumber();
        auto to = reader.readPhoneNumber();

        switch (msgId)
        {
        case common::MessageId::Sib:
        {
            auto btsId = reader.readBtsId();
            handler->handleSib(btsId);
            break;
        }
        case common::MessageId::AttachResponse:
        {
            bool accept = reader.readNumber<std::uint8_t>() != 0u;
            if (accept)
                handler->handleAttachAccept();
            else
                handler->handleAttachReject();
            break;
        }
        case common::MessageId::CallRequest:
        {
            handler->handleCallRequest(from);
            break;
        }
        case common::MessageId::CallAccepted:
        {
            handler->handleCallAccepted(from);
            break;
        }
        case common::MessageId::CallDropped:
        {
            handler->handleCallDropped(from);
            break;
        }
        case common::MessageId::Sms:
        {
            handler->handleSMS(from, reader.readRemainingText(),common::MessageId::Sms);
            break;
        }
        case common::MessageId::CallTalk:
        {
            break;
        }
        case common::MessageId::UnknownRecipient:
        {
            auto failMsgId = reader.readMessageId();
            switch (failMsgId) {
                case common::MessageId::Sms:
                    logger.logError("unknown message: ", msgId, ", to: ",to);
                    handler->handleSMS(from, reader.readRemainingText(),
                                       common::MessageId::UnknownRecipient);
                    break;
                case common::MessageId::CallRequest:
                    handler->handleUnknownRecipientAfterCallRequest();
                    break;
                case common::MessageId::CallAccepted:
                    handler->handleUnknownRecipientAfterCallAccepted();
                    break;
            }
            break;
        }
        default:
            logger.logError("unknown message: ", msgId, ", from: ", from);
        }
    }
    catch (std::exception const& ex)
    {
        logger.logError("handleMessage error: ", ex.what());
    }
}


void BtsPort::sendAttachRequest(common::BtsId btsId)
{
    logger.logDebug("sendAttachRequest: ", btsId);
    common::OutgoingMessage msg{common::MessageId::AttachRequest,
                                phoneNumber,
                                common::PhoneNumber{}};
    msg.writeBtsId(btsId);
    transport.sendMessage(msg.getMessage());


}

void BtsPort::sendCallDropped(common::PhoneNumber to)
{
    common::OutgoingMessage msg{common::MessageId::CallDropped,
                                phoneNumber,
                                to};
    transport.sendMessage(msg.getMessage());
}


void BtsPort::sendCallAccepted(common::PhoneNumber to)
{
    common::OutgoingMessage msg{common::MessageId::CallAccepted,
                                phoneNumber,
                                to};
    transport.sendMessage(msg.getMessage());
}

void BtsPort::sendCallRequest(common::PhoneNumber to)
{
    common::OutgoingMessage msg{common::MessageId::CallRequest,
                                phoneNumber,
                                to};
    transport.sendMessage(msg.getMessage());
}

void BtsPort::sendSMS(common::PhoneNumber to, std::string msg)
{
    auto outgoingMsg = common::OutgoingMessage(common::MessageId::Sms,phoneNumber,to);
    outgoingMsg.writeText(msg);
    transport.sendMessage(outgoingMsg.getMessage());
}


}
