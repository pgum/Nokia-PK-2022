#pragma once

#include "Messages/PhoneNumber.hpp"

namespace ue {

class ITextMessage {
public:
    virtual std::string
    getMessageSummary() = 0;        // let summary be less than 16 characters long (for optimization)
    virtual std::string getMessage() = 0;               // get full message
    virtual common::PhoneNumber getFromNumber() = 0;

    virtual common::PhoneNumber getToNumber() = 0;
};

}