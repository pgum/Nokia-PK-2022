#pragma once

#include "ISMSDatabase.hpp"
#include "SMS.hpp"

namespace ue
{

class BasicSMSDatabase : public ISMSDatabase
{
public:
    BasicSMSDatabase();
    void addSMS(common::PhoneNumber from, common::PhoneNumber to, std::string message, bool isRead = false, SmsTransmissionState state = initial) override;
    void addSMS(ITextMessage& sms) override;
    ITextMessage& getSMS(unsigned int id) override;
    std::optional<std::shared_ptr<ITextMessage>> getLastSMSSend() override;
    const std::vector<std::pair<unsigned int, std::shared_ptr<ITextMessage>>> & getAllSMS() override; //returns summaries

private:
    std::vector<std::pair<unsigned int,std::shared_ptr< ITextMessage>>> data;
    unsigned int nextId;
};

}
