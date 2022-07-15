#include "BtsPort.hpp"
#include "Messages/IncomingMessage.hpp"
#include "Messages/OutgoingMessage.hpp"

namespace ue {

    BtsPort::BtsPort(common::ILogger &logger, common::ITransport &transport, common::PhoneNumber phoneNumber)
            : logger(logger, "[BTS-PORT]"),
              transport(transport),
              phoneNumber(phoneNumber) {}

    void BtsPort::start(IBtsEventsHandler &handler) {
        this->handler = &handler;
        transport.registerMessageCallback([this](BinaryMessage msg) { handleMessage(msg); });
        transport.registerDisconnectedCallback([this] { this->handler->handleDisconnected(); });
    }

    void BtsPort::stop() {
        transport.registerDisconnectedCallback(nullptr);
        transport.registerMessageCallback(nullptr);
        handler = nullptr;
    }

    void BtsPort::handleMessage(BinaryMessage msg) {
        try {
            common::IncomingMessage reader{msg};
            auto msgId = reader.readMessageId();
            auto from = reader.readPhoneNumber();
            auto to = reader.readPhoneNumber();

            switch (msgId) {
                case common::MessageId::Sib: {
                    auto btsId = reader.readBtsId();
                    handler->handleSib(btsId);
                    break;
                }
                case common::MessageId::AttachResponse: {
                    bool accept = reader.readNumber<std::uint8_t>() != 0u;
                    if (accept)
                        handler->handleAttachAccept();
                    else
                        handler->handleAttachReject();
                    break;
                }
                case common::MessageId::Sms: {
                    auto action = reader.readNumber<std::uint8_t>();
                    if (action == 0) {
                        std::string text = reader.readRemainingText();
                        handler->handleSmsReceive(action, text, from, to);
                    }
                    break;
                }
                case common::MessageId::CallRequest: {
                    handler->handleCallRequest(from);
                    break;
                }
                case common::MessageId::CallDropped: {
                    handler->handleCallDrop(from);
                    break;
                }
                case common::MessageId::CallAccepted: {
                    handler->handleCallAccept(from);
                    break;
                }
                case common::MessageId::CallTalk:
                    handler->handleCallTalk(from, reader.readRemainingText());
                    break;
                case common::MessageId::UnknownRecipient: {
                    auto failHeader = reader.readMessageHeader();
                    if(failHeader.messageId == common::MessageId::Sms)handler->handleFailedSmsSend();
                    else if(failHeader.messageId == common::MessageId::CallTalk)handler->handleUnknownRecipientCallTalk(from);
                    else handler->handleUnknownRecipientCallRequest(from);
                    logger.logInfo("Handle for unknown recipient");
                    break;
                }
                default: {
                    logger.logError("message handler for ", msgId, "not implemented(from ", from, ")");
                }

            }
        }
        catch (std::exception const &ex) {
            logger.logError("handleMessage error: ", ex.what());
        }
    }

    void BtsPort::sendSms(common::PhoneNumber toPhoneNumber, std::string text) {

        common::OutgoingMessage outgoingMessage = common::OutgoingMessage(common::MessageId::Sms, phoneNumber,
                                                                          toPhoneNumber);
        outgoingMessage.writeNumber(static_cast<uint8_t>(0));
        outgoingMessage.writeText(text);

        transport.sendMessage(outgoingMessage.getMessage());
    }


    common::PhoneNumber BtsPort::getOwnPhoneNumber() {
        return phoneNumber;
    }


    void BtsPort::sendAttachRequest(common::BtsId btsId) {
        logger.logDebug("sendAttachRequest: ", btsId);
        common::OutgoingMessage msg{common::MessageId::AttachRequest,
                                    phoneNumber,
                                    common::PhoneNumber{}};
        msg.writeBtsId(btsId);
        transport.sendMessage(msg.getMessage());
    }

    void BtsPort::sendCallAccept(common::PhoneNumber receiverPhoneNumber) {
        logger.logDebug("sendCallAccept: ", receiverPhoneNumber);
        common::OutgoingMessage msg{common::MessageId::CallAccepted,
                                    phoneNumber,
                                    receiverPhoneNumber};
        transport.sendMessage(msg.getMessage());
    }

    void BtsPort::sendCallDrop(common::PhoneNumber receiverPhoneNumber) {
        logger.logDebug("sendCallDrop: ", receiverPhoneNumber);
        common::OutgoingMessage msg{common::MessageId::CallDropped,
                                    phoneNumber,
                                    receiverPhoneNumber};
        transport.sendMessage(msg.getMessage());
    }

    void BtsPort::sendCallRequest(common::PhoneNumber receiverPhoneNumber) {
        common::OutgoingMessage msg{common::MessageId::CallRequest,
                                    phoneNumber,
                                    receiverPhoneNumber};
        transport.sendMessage(msg.getMessage());
    }

    void BtsPort::sendCallTalk(common::PhoneNumber receiverPhoneNumber, std::string message)
    {
        common::OutgoingMessage outgoingMsg{common::MessageId::CallTalk,
                                            phoneNumber,
                                            receiverPhoneNumber};
        outgoingMsg.writeText(message);
        transport.sendMessage(outgoingMsg.getMessage());
    }

}
