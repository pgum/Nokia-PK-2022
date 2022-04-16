#pragma once

#include <vector>

#include "Messages/PhoneNumber.hpp"
#include "SMSDB/SMS.hpp"

namespace ue
{

class ISMSDatabase{
public:
    virtual void addSMS(common::PhoneNumber from, common::PhoneNumber to, std::string message) = 0;
    virtual ue::SMS getSMS(int id) = 0;
    virtual std::vector<std::string> viewAllSMS() = 0;

private:
    int nextId;
};

}
