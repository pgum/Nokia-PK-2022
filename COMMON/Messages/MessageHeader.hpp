#pragma once

#include <ostream>

#include "Messages/MessageId.hpp"
#include "Messages/PhoneNumber.hpp"

namespace common
{

struct MessageHeader
{
    MessageId messageId;
    PhoneNumber from;
    PhoneNumber to;
};

std::ostream& operator << (std::ostream&, const MessageHeader&);

}
