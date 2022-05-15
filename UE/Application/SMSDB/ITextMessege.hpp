#pragma once

#include "Messages/PhoneNumber.hpp"

namespace
{
    enum SmsTransmissionState
    {
        initial,
        SuccessfullySend,
        Bounce
    };
}

namespace ue {

class ITextMessage {
public:
    virtual std::string
    getMessageSummary() = 0;        // let summary be less than 16 characters long (for optimization)
    virtual std::string getMessage() = 0;               // get full message
    virtual common::PhoneNumber getFromNumber() = 0;

    virtual common::PhoneNumber getToNumber() = 0;
    virtual void setIsReadStatus(bool status) = 0;
    virtual bool getIsReadStatus() = 0;
    virtual void setSMSTransmissionState(SmsTransmissionState state) = 0;
    virtual SmsTransmissionState getSMSTransmissionState() = 0;
};

}