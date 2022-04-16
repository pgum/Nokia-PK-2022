#include "BasicSMSDatabase.hpp"

#include <stdexcept> //delete after implementation

namespace ue
{
    BasicSMSDatabase::BasicSMSDatabase()
    {
        throw std::logic_error("Not implemented!");
    }

    void BasicSMSDatabase::addSMS(common::PhoneNumber from, common::PhoneNumber to, std::string message)
    {
        throw std::logic_error("Not implemented!");
    }

    ue::SMS BasicSMSDatabase::getSMS(int id)
    {
        throw std::logic_error("Not implemented!");
    }

    std::vector<std::string> BasicSMSDatabase::viewAllSMS()
    {
        throw std::logic_error("Not implemented!");
    }
}
