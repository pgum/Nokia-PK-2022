#include "BasicSMSDatabase.hpp"

namespace ue
{


    BasicSMSDatabase::BasicSMSDatabase()
        : nextId{0}
    {
    }

    void BasicSMSDatabase::addSMS(common::PhoneNumber from, common::PhoneNumber to, std::string message, bool isRead)
    {
        data.emplace_back(
                nextId++, std::make_unique<SMS>(from,to,message,isRead)
                );
    }

    ITextMessage& BasicSMSDatabase::getSMS(unsigned int id)
    {
        return *data.at(id).second;
    }

    const std::vector<std::pair<unsigned int, std::shared_ptr< ITextMessage>>>& BasicSMSDatabase::getAllSMS()
    {
        return data;
    }

    ITextMessage &BasicSMSDatabase::getLastSMSSend()
    {
        return *data.end()->second;
    }
}
