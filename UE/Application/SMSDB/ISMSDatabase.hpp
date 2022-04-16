#pragma once

#include "Messages/PhoneNumber.hpp"

namespace ue
{

class ISMSDatabase{
public:
    virtual void addSMS(common::PhoneNumber from, common::PhoneNumber to, std::string message) = 0;
    virtual void getSMS(int id) = 0;
    virtual std::vector<std::string> viewAllSMS() = 0;
};

}
