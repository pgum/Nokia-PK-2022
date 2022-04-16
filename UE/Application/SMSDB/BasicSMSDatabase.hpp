#pragma once

#include "ISMSDatabase.hpp"

namespace ue
{

class BasicSMSDatabase : ISMSDatabase{
public:
    BasicSMSDatabase();
    void addSMS(common::PhoneNumber from, common::PhoneNumber to, std::string message) override;
};

}
