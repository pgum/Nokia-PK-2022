#pragma once

#include <vector>

#include "Messages/PhoneNumber.hpp"
#include "SMSDB/ITextMessege.hpp"
#include <memory>
#include <optional>

namespace ue
{

class ISMSDatabase{
public:
    virtual ~ISMSDatabase() = default;

    virtual void addSMS(common::PhoneNumber from, common::PhoneNumber to, std::string message, bool isRead = false, SmsTransmissionState state = initial) = 0;
    virtual void addSMS(ITextMessage& sms) = 0;
    virtual ITextMessage& getSMS(unsigned int id) = 0;
    virtual std::optional<std::shared_ptr<ITextMessage>> getLastSMSSend() = 0;
    virtual const std::vector<std::pair<unsigned int, std::shared_ptr<ITextMessage>>>& getAllSMS() = 0;
};

}
