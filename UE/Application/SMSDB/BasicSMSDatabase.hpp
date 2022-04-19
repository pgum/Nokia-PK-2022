#pragma once

#include "ISMSDatabase.hpp"
#include "SMS.hpp"

namespace ue
{

class BasicSMSDatabase : ISMSDatabase{
public:
    BasicSMSDatabase();
    void addSMS(common::PhoneNumber from, common::PhoneNumber to, std::string message) override;
    ue::SMS getSMS(unsigned int id) override;
    std::vector<std::pair<unsigned int, ue::SMS>> getAllSMS() override; //returns summaries

private:
    std::vector<std::pair<unsigned int, ue::SMS>> data;
    unsigned int nextId;
};

}
