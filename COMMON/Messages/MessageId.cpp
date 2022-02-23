#include "MessageId.hpp"

namespace common
{

std::istream& operator >> (std::istream& is, MessageId& id)
{
    std::string text;
    if (not (is >> text))
    {
        return is;
    }
#define TRY_MESSAGE_ID(ID) if (#ID == text) { id = MessageId::ID; return is; }
    FOR_ALL_MESSAGE_IDS(TRY_MESSAGE_ID);
#undef  TRY_MESSAGE_ID

    is.setstate(std::ios_base::failbit);
    return is;
}

std::ostream& operator << (std::ostream& os, const MessageId& id)
{
#define CASE_FOR_ID(id) case MessageId::id: return os << #id;
    switch (id)
    {
        FOR_ALL_MESSAGE_IDS(CASE_FOR_ID)
    default:
        return os << "Unknown(" << std::uint32_t(get(id)) << ")";
    };
#undef CASE_FOR_ID
}

std::string to_string(const MessageId& id)
{
#define CASE_FOR_ID(id) case MessageId::id: return #id;
    switch (id)
    {
        FOR_ALL_MESSAGE_IDS(CASE_FOR_ID)
    default:
        return "Unknown(" + std::to_string(std::uint32_t(get(id))) + ")";
    };
#undef CASE_FOR_ID
}

}
