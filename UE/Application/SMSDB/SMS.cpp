#include "SMS.hpp"

#include <stdexcept> //delete after implementation

namespace ue
{
    SMS::SMS(common::PhoneNumber from, common::PhoneNumber to, std::string message)
    {
        throw std::logic_error("Not implemented!");
    }

    SMS::SMS(const SMS& sms)
    {
        throw std::logic_error("Not implemented!");
    }

    SMS& SMS::operator=(const SMS& sms)
    {
        throw std::logic_error("Not implemented!");
    }

    std::string SMS::getMessageSummary()
    {
        throw std::logic_error("Not implemented!");
    }

    std::string SMS::getMessage()
    {
        throw std::logic_error("Not implemented!");
    }

    common::PhoneNumber SMS::getFromNumber()
    {
        throw std::logic_error("Not implemented!");
    }

    common::PhoneNumber SMS::getToNumber()
    {
        throw std::logic_error("Not implemented!");
    }
}
