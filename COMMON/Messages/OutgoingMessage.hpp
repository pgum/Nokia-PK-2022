#pragma once

#include "Messages/MessageHeader.hpp"
#include "Messages/BinaryMessage.hpp"
#include "Messages/BtsId.hpp"
#include <stdexcept>
#include <string>
#include <algorithm>
#include <iterator>

namespace common
{

class OutgoingMessage
{
public:
    class WriteEx : public std::logic_error
    {
    public:
        using std::logic_error::logic_error;
    };

    OutgoingMessage(MessageId messageId, PhoneNumber from, PhoneNumber to);
    OutgoingMessage();

    template <typename T>
    void writeNumber(T number);
    void writeNumber(bool value);
    void writeBtsId(const BtsId& btsId);
    void writePhoneNumber(const PhoneNumber& phoneNumber);
    void writeMessageId(MessageId messageId);
    void writeText(const std::string&);

    void writeMessageHeader(const MessageHeader& messageHeader);

    BinaryMessage getMessage();

private:
    BinaryMessage message;
};

template <typename T>
void OutgoingMessage::writeNumber(T number)
{
    static_assert(std::is_unsigned<T>::value, "Must be unsigned number");

    std::uint8_t bytes[sizeof(T)];

    for (std::size_t i = 0u; i < sizeof(T); ++i)
    {
        bytes[sizeof(T) - i - 1] = (number & 0xFF);
        number >>= 8u;
    }
    std::copy(std::begin(bytes), std::end(bytes), std::back_inserter(message.value));
}

}
