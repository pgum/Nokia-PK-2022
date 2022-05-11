#include "BasicSMSDatabase.hpp"

namespace ue
{


    BasicSMSDatabase::BasicSMSDatabase()
        : nextId{0}
    {
    }

    void BasicSMSDatabase::addSMS(common::PhoneNumber from, common::PhoneNumber to, std::string message)
    {
        data.emplace_back(
                nextId++, std::make_unique<SMS>(from,to,message)
                );
    }

    ITextMessage& BasicSMSDatabase::getSMS(unsigned int id)
    {
        return *data.at(id).second;
    }

    const std::vector<std::pair<unsigned int, std::unique_ptr< ITextMessage>>>& BasicSMSDatabase::getAllSMS()
    {
        return data;
    }
}
