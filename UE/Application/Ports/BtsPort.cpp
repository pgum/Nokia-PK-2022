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
        transport.registerDisconnectedCallback([this] {this->handler->handleDisconnected(); });
        this->handler = &handler;
    }

    void BtsPort::stop()
    {
        transport.registerDisconnectedCallback(nullptr);
        transport.registerMessageCallback(nullptr);
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

                case common::MessageId::Sms:
                {
                    std::string text = reader.readRemainingText();
                    handler->handleSMSReceive(text,from);
                    break;
                }

                case common::MessageId::CallRequest:
                {
                    std::string text = reader.readRemainingText();
                    handler->handleCallRequest(from);
                    break;
                }

                case common::MessageId::CallDropped:
                {
                    std::string text = reader.readRemainingText();
                    handler->handleDropCall(from);
                    break;
                }

                default:
                    logger.logError("unknow message: ", msgId, ", from: ", from);

            }
        }
        catch (std::exception const& ex)
        {
            logger.logError("handleMessage error: ", ex.what());
        }
    }

    void BtsPort::sendSms(common::PhoneNumber receiverPhoneNumber, std::string smsText) {
        common::OutgoingMessage outgoingMessage = common::OutgoingMessage(common::MessageId::Sms, phoneNumber, receiverPhoneNumber);
        outgoingMessage.writeText(smsText);
        transport.sendMessage(outgoingMessage.getMessage());
    }

    void BtsPort::makeCall(common::PhoneNumber receiverPhoneNumber) {
        common::OutgoingMessage outgoingMessage{common::MessageId::CallRequest,phoneNumber,receiverPhoneNumber};
        transport.sendMessage(outgoingMessage.getMessage());
    }

    void BtsPort::declineCall(common::PhoneNumber callerPhoneNumber) {
        common::OutgoingMessage outgoingMessage{common::MessageId::CallDropped,phoneNumber,callerPhoneNumber};
        transport.sendMessage(outgoingMessage.getMessage());
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

}
