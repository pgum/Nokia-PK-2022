#pragma once

#include "Messages/PhoneNumber.hpp"
#include "ITextMessege.hpp"

namespace
{
#define MAX_SUMMARY_SIZE 15
}

namespace ue{

class SMS : public ITextMessage
{
public:
    SMS();
    SMS(common::PhoneNumber from, common::PhoneNumber to, std::string message, bool isRead, SmsTransmissionState state = initial);
    SMS(const SMS &sms);
    SMS &operator =(const SMS &sms);

    std::string getMessageSummary() override;
    std::string getMessage() override;
    common::PhoneNumber getFromNumber() override;
    common::PhoneNumber getToNumber() override;
    void setSMSTransmissionState(SmsTransmissionState state) override;
    SmsTransmissionState getSMSTransmissionState() override;
    void setIsReadStatus(bool status) override;
    bool getIsReadStatus() override;

private:
    std::string message;
    common::PhoneNumber from;
    common::PhoneNumber to;
    bool isRead = false;
    SmsTransmissionState smsTransmissionState;
};

}
