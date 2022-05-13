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
    SMS(common::PhoneNumber from, common::PhoneNumber to, std::string message, bool isRead);
    SMS(const SMS &sms);
    SMS &operator =(const SMS &sms);

    std::string getMessageSummary() override;
    std::string getMessage() override;
    common::PhoneNumber getFromNumber() override;
    common::PhoneNumber getToNumber() override;

private:
    std::string message;
    common::PhoneNumber from;
    common::PhoneNumber to;
    bool isRead = false;
};

}
