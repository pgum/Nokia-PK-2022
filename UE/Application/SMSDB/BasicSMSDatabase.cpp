#include "BasicSMSDatabase.hpp"

#include <stdexcept> //delete after implementation

namespace ue
{


    BasicSMSDatabase::BasicSMSDatabase()
        : nextId{0}
    {
    }

    void BasicSMSDatabase::addSMS(common::PhoneNumber from, common::PhoneNumber to, std::string message)
    {
        data.push_back(
                std::pair<unsigned int, ue::SMS>
                        {nextId++,SMS{from,to,message}}
                        );
    }

    ue::SMS BasicSMSDatabase::getSMS(unsigned int id)
    {
        return data.at(id).second;
    }

    std::vector<std::pair<unsigned int, ue::SMS>> BasicSMSDatabase::getAllSMS()
    {
        return data;
    }
}
