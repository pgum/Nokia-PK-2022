#pragma once

#include <vector>

#include "Messages/PhoneNumber.hpp"
#include "SMSDB/ITextMessege.hpp"
#include <memory>

namespace ue
{

class ISMSDatabase{
public:
    virtual ~ISMSDatabase() = default;

    virtual void addSMS(common::PhoneNumber from, common::PhoneNumber to, std::string message, bool isRead = false) = 0;
    virtual ITextMessage& getSMS(unsigned int id) = 0;
    virtual ITextMessage& getLastSMSSend() = 0;
    virtual const std::vector<std::pair<unsigned int, std::unique_ptr<ITextMessage>>>& getAllSMS() = 0;
};

}
