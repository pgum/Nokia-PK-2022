#pragma once

#include "Messages/BinaryMessage.hpp"
#include "Messages/MessageHeader.hpp"
#include "Messages/BtsId.hpp"
#include <stdexcept>
#include <string>
#include <memory>

namespace common
{

class IncomingMessage
{
public:
    class ReadEx : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    IncomingMessage(const BinaryMessage& message);

    template<typename T>
    static std::enable_if_t<not std::is_pointer<T>::value, IncomingMessage> create(const T& message)
    {
        return IncomingMessage(&message);
    }

    template<typename T>
    static IncomingMessage create(const std::unique_ptr<T>& message)
    {
        return IncomingMessage(message->data(), message->size());
    }

    template<typename T>
    static std::enable_if_t<std::is_pointer<T>::value, IncomingMessage> create(const T& message)
    {
        return IncomingMessage(message->data(), message->size());
    }

    template <typename T>
    T readNumber();

    MessageId readMessageId();
    BtsId readBtsId();
    PhoneNumber readPhoneNumber();
    std::string readText(std::size_t);
    std::string readRemainingText();
    MessageHeader readMessageHeader();

    void checkEndOfMessage();
private:
    using Cursor = BinaryMessage::Value::const_iterator;
    std::string readTextTo(Cursor position);

    Cursor cursor;
    const Cursor end;
};

template <typename T>
T IncomingMessage::readNumber()
{
    T number{};
    std::size_t i = 0u;
    for (; i < sizeof(T) && cursor != end; ++cursor, ++i)
    {
        number =  (number << 8u) + *cursor;
    }
    if (i != sizeof(T))
    {
        throw ReadEx("Cannot read " + std::to_string(sizeof(T)) + " bytes");
    }
    return number;
}

template <>
bool IncomingMessage::readNumber<bool>();

}
