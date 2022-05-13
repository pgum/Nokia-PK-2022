#include "SMS.hpp"

#include <stdexcept> //delete after implementation

namespace ue
{
    SMS::SMS()
    {
        from = common::PhoneNumber{0};
        to = common::PhoneNumber{0};
        message = "";
        isRead = false;
    }

    SMS::SMS(common::PhoneNumber from, common::PhoneNumber to, std::string message, bool is_read)
    {
        this->from = from;
        this->to = to;
        this->message = message;
        this->isRead = is_read;
    }

    SMS::SMS(const SMS& other)
    {
        this->to = other.to;
        this->from = other.from;
        this->message = other.message;
        this->isRead = other.isRead;
    }

    SMS& SMS::operator=(const SMS& other)
    {
        if(this == &other)
            return *this;

        this->to = other.to;
        this->from = other.from;
        this->message = other.message;
        this->isRead = other.isRead;

        return *this;
    }

    std::string SMS::getMessageSummary()
    {
        if(isRead)
            return message.substr(0,MAX_SUMMARY_SIZE);
        else
            return "*"+message.substr(0,MAX_SUMMARY_SIZE-1);
    }

    std::string SMS::getMessage()
    {
        isRead = true;
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
