#pragma once

#include <cstdint>
#include <type_traits>
#include <iostream>
#include <string>

namespace common
{

#define FOR_ALL_MESSAGE_IDS(ACTION) \
    ACTION(Sib)                     \
    ACTION(AttachRequest)           \
    ACTION(AttachResponse)          \
    ACTION(UnknownRecipient)        \
    ACTION(UnknownSender)           \
    ACTION(Sms)                     \
    ACTION(CallRequest)             \
    ACTION(CallAccepted)            \
    ACTION(CallDropped)             \
    ACTION(CallTalk)                \

#define MESSAGE_ID_ENTRY(X) X,
enum class MessageId : std::uint8_t
{
    FOR_ALL_MESSAGE_IDS(MESSAGE_ID_ENTRY)
};
#undef MESSAGE_ID_ENTRY

constexpr auto get(MessageId messageId)
{
    return static_cast<std::underlying_type_t<MessageId>>(messageId);
}

std::istream& operator >> (std::istream&, MessageId&);
std::ostream& operator << (std::ostream&, const MessageId&);
std::string to_string(const MessageId&);

}
