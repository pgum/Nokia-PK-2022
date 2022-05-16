#include "SMS.hpp"


namespace ue
{
    SMS::SMS()
    {
        from = common::PhoneNumber{0};
        to = common::PhoneNumber{0};
        message = "";
        isRead = false;
    }

    SMS::SMS(common::PhoneNumber from, common::PhoneNumber to, std::string message, bool is_read, SmsTransmissionState state)
    {
        this->from = from;
        this->to = to;
        this->message = message;
        this->isRead = is_read;
        this->smsTransmissionState = state;
    }

    SMS::SMS(const SMS& other)
    {
        this->to = other.to;
        this->from = other.from;
        this->message = other.message;
        this->isRead = other.isRead;
        this->smsTransmissionState = other.smsTransmissionState;
    }

    SMS& SMS::operator=(const SMS& other)
    {
        if(this == &other)
            return *this;

        this->to = other.to;
        this->from = other.from;
        this->message = other.message;
        this->isRead = other.isRead;
        this->smsTransmissionState = other.smsTransmissionState;

        return *this;
    }

    std::string SMS::getMessageSummary()
    {
        int pos = message.find_first_of("\n");
        if(pos==-1 || pos>MAX_SUMMARY_SIZE) pos = MAX_SUMMARY_SIZE;

        switch(smsTransmissionState)
        {
            case Bounce:
            {
                return "SEND ERR: " + message.substr(0, pos - 10);
            }
            case Received:
            {
                if (isRead)
                    return message.substr(0, pos);
                else
                    return "*" + message.substr(0, pos - 1);
            }
            case Send:
            {
                return message.substr(0, pos);
            }
            case initial:
            {
                if (isRead)
                    return message.substr(0, pos);
                else
                    return "*" + message.substr(0, pos - 1);
            }
            default:
            {
                return "unhandled-case";
            }
        }
    }

    std::string SMS::getMessage()
    {
        return message;
    }

    common::PhoneNumber SMS::getFromNumber()
    {
        return from;
    }

    common::PhoneNumber SMS::getToNumber()
    {
        return to;
    }

    void SMS::setSMSTransmissionState(SmsTransmissionState state)
    {
        smsTransmissionState = state;
    }

    SmsTransmissionState SMS::getSMSTransmissionState()
    {
        return smsTransmissionState;
    }

    void SMS::setIsReadStatus(bool status)
    {
        isRead = status;
    }

    bool SMS::getIsReadStatus()
    {
        return isRead;
    }
}
