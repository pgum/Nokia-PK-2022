#include "OutgoingMessage.hpp"
#include <algorithm>
#include <iterator>

namespace common
{

OutgoingMessage::OutgoingMessage(MessageId messageId, PhoneNumber from, PhoneNumber to)
{
    writeMessageHeader(MessageHeader{messageId, from, to});
}

OutgoingMessage::OutgoingMessage()
{}

void OutgoingMessage::writeNumber(bool value)
{
    writeNumber<std::uint8_t>(value ? 1 : 0);
}

void OutgoingMessage::writeBtsId(const BtsId &btsId)
{
    writeNumber(btsId.value);
}

void OutgoingMessage::writePhoneNumber(const PhoneNumber &phoneNumber)
{
    writeNumber(phoneNumber.value);
}

void OutgoingMessage::writeMessageId(MessageId messageId)
{
    writeNumber(get(messageId));
}

void OutgoingMessage::writeText(const std::string &text)
{
    std::copy(text.begin(), text.end(), std::back_inserter(message.value));
}

void OutgoingMessage::writeMessageHeader(const MessageHeader &messageHeader)
{
    writeMessageId(messageHeader.messageId);
    writePhoneNumber(messageHeader.from);
    writePhoneNumber(messageHeader.to);
}

BinaryMessage OutgoingMessage::getMessage()
{
    return message;
}

}
