#include "SMS.hpp"

#include <stdexcept> //delete after implementation

namespace ue
{
    SMS::SMS()
    {
        from = common::PhoneNumber{0};
        to = common::PhoneNumber{0};
        message = "";

    }

    SMS::SMS(common::PhoneNumber from, common::PhoneNumber to, std::string message)
    {
        this->from = from;
        this->to = to;
        this->message = message;
    }

    SMS::SMS(const SMS& other)
    {
        this->to = other.to;
        this->from = other.from;
        this->message = other.message;
    }

    SMS& SMS::operator=(const SMS& other)
    {
        if(this == &other)
            return *this;

        this->to = other.to;
        this->from = other.from;
        this->message = other.message;

        return *this;
    }

    std::string SMS::getMessageSummary()
    {
        return message.substr(0,MAX_SUMMARY_SIZE);
    }

    std::string SMS::getMessage()
    {
        return message;
    }

    common::PhoneNumber SMS::getFromNumber()
    {
        return from;
    }

    common::PhoneNumber SMS::getToNumber()
    {
        return to;
    }
}
