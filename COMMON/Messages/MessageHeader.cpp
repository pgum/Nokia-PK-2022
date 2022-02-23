#include "MessageHeader.hpp"

namespace common
{

std::ostream &operator <<(std::ostream &os, const MessageHeader &obj)
{
    return os << "message: " << obj.messageId << ", from: " << obj.from << ", to: " << obj.to;
}

}
