#include "IncomingMessage.hpp"
#include <algorithm>

namespace common
{

IncomingMessage::IncomingMessage(const BinaryMessage &message)
    : cursor(message.value.begin()),
      end(message.value.end())
{}

MessageHeader IncomingMessage::readMessageHeader()
{
    MessageHeader messageHeader;
    messageHeader.messageId = readMessageId();
    messageHeader.from = readPhoneNumber();
    messageHeader.to = readPhoneNumber();

    return messageHeader;
}

MessageId IncomingMessage::readMessageId()
{
    using MessageIdType = std::underlying_type_t<MessageId>;
    MessageIdType value = readNumber<MessageIdType>();

#define MESSAGE_ID_CASE(X) case get(MessageId::X): return MessageId::X;
    switch (value)
    {
        FOR_ALL_MESSAGE_IDS(MESSAGE_ID_CASE)
    default:
        throw ReadEx("MessageId value out of range: "
                     + std::to_string(static_cast<std::uint32_t>(value)));
    }
#undef MESSAGE_ID_CASE
}

template <>
bool IncomingMessage::readNumber<bool>()
{
    return readNumber<std::uint8_t>() != 0;
}

PhoneNumber IncomingMessage::readPhoneNumber()
{
    return PhoneNumber{ readNumber<decltype(PhoneNumber::value)>() };
}

BtsId IncomingMessage::readBtsId()
{
    return BtsId{ readNumber<decltype(BtsId::value)>() };
}

std::string IncomingMessage::readText(std::size_t textLength)
{
    const auto remainingBytes = std::distance(cursor, end);
    if (textLength > remainingBytes)
    {
        throw ReadEx("Cannot read " + std::to_string(textLength) + "bytes");
    }
    return readTextTo(cursor + textLength);
}

std::string IncomingMessage::readRemainingText()
{
    return readTextTo(end);
}

void IncomingMessage::checkEndOfMessage()
{
    if (cursor != end)
    {
        throw ReadEx("Still something to read: " + std::to_string(std::distance(cursor, end)));
    }
}

std::string IncomingMessage::readTextTo(Cursor end)
{
    std::string text(cursor, end);
    cursor = end;
    return text;
}

}
