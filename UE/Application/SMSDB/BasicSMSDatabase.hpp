#pragma once

#include "ISMSDatabase.hpp"
#include "SMS.hpp"

namespace ue
{

class BasicSMSDatabase : public ISMSDatabase
{
public:
    BasicSMSDatabase();
    void addSMS(common::PhoneNumber from, common::PhoneNumber to, std::string message) override;
    ITextMessage& getSMS(unsigned int id) override;
    const std::vector<std::pair<unsigned int, std::unique_ptr<ITextMessage>>> & getAllSMS() override; //returns summaries

private:
    std::vector<std::pair<unsigned int,std::unique_ptr< ITextMessage>>> data;
    unsigned int nextId;
};

}
