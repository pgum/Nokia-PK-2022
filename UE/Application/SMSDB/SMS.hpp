#pragma once

#include "Messages/PhoneNumber.hpp"

namespace ue{

class SMS{
public:
    SMS(common::PhoneNumber from, common::PhoneNumber to, std::string message);
    std::string getMessageSummary();                    // let summary be less than 16 characters long (for optimization)
    std::string getMessage();                           // get full message
    common::PhoneNumber getFromNumber();
    common::PhoneNumber getToNumber();

private:
    static int nextId = 0;
    int id;
    // If anyone have better idea than int id and initializing it with static, go ahead!

    std::string message;
    common::PhoneNumber from;
    common::PhoneNumber to;
};

}
