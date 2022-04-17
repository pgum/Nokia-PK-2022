#pragma once

#include "ISMSDatabase.hpp"
#include "SMS.hpp"

namespace ue
{

class BasicSMSDatabase : ISMSDatabase{
public:
    BasicSMSDatabase();
    void addSMS(common::PhoneNumber from, common::PhoneNumber to, std::string message) override;
    ue::SMS getSMS(int id) override;
    std::vector<std::string> viewAllSMS() override; //returns summaries

private:
    int nextId;
};

}
