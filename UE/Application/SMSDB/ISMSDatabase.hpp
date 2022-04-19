#pragma once

#include <vector>

#include "Messages/PhoneNumber.hpp"
#include "SMSDB/SMS.hpp"

namespace ue
{

class ISMSDatabase{
public:
    virtual void addSMS(common::PhoneNumber from, common::PhoneNumber to, std::string message) = 0;
    virtual ue::SMS getSMS(unsigned int id) = 0;
    virtual std::vector<std::pair<unsigned int, ue::SMS>> getAllSMS() = 0;
};

}
