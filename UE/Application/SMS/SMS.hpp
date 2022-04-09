#include <string>
#include "Messages/MessageId.hpp"
#include "Messages/PhoneNumber.hpp"
#pragma once


class SMS{
    common::MessageId m_messageID;
    common::PhoneNumber m_sender;
    common::PhoneNumber m_receiver;
    std::string m_textMessage;
    bool m_isViewed;

public:
    void Set_Viewed_Value();

};