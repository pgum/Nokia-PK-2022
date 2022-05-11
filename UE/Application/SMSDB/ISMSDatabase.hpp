#pragma once

#include <vector>

#include "Messages/PhoneNumber.hpp"
#include "SMSDB/ITextMessege.hpp"
#include <memory>

namespace ue
{

class ISMSDatabase{
public:
    virtual void addSMS(common::PhoneNumber from, common::PhoneNumber to, std::string message) = 0;
    virtual ITextMessage& getSMS(unsigned int id) = 0;
    virtual const std::vector<std::pair<unsigned int, std::unique_ptr<ITextMessage>>>& getAllSMS() = 0;
};

}
