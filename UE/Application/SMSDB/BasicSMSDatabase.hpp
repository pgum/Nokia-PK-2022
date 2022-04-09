#pragma once

#include "ISMSDatabase.hpp"

namespace ue
{

class BasicSMSDatabase : ISMSDatabase{
public:
    BasicSMSDatabase();
    void AddMessage(common::PhoneNumber phoneNumber, std::string message) override;
};

}
