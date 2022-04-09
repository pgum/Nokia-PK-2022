#pragma once

#include "Messages/PhoneNumber.hpp"

namespace ue
{

class ISMSDatabase{
public:
    virtual void AddMessage(common::PhoneNumber phoneNumber, std::string message) = 0;
};

}
