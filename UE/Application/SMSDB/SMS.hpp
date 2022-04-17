#pragma once

#include "Messages/PhoneNumber.hpp"

namespace
{
#define MAX_SUMMARY_SIZE 15
}

namespace ue{

class SMS{
public:
    SMS();
    SMS(common::PhoneNumber from, common::PhoneNumber to, std::string message);
    SMS(const SMS &sms);
    SMS &operator =(const SMS &sms);

    std::string getMessageSummary();                    // let summary be less than 16 characters long (for optimization)
    std::string getMessage();                           // get full message
    common::PhoneNumber getFromNumber();
    common::PhoneNumber getToNumber();

private:
    std::string message;
    common::PhoneNumber from;
    common::PhoneNumber to;
};

}
