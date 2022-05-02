#include <string>
#include "Messages/MessageId.hpp"
#include "Messages/PhoneNumber.hpp"

#pragma once

class SMS {
    common::MessageId m_messageID;
    common::PhoneNumber m_sender;
    common::PhoneNumber m_receiver;
    std::string m_textMessage;
    bool m_isViewed;
public:
    SMS(common::MessageId mMessageId, const common::PhoneNumber &mSender, const common::PhoneNumber &mReceiver,
        const std::string &mTextMessage);

    SMS();

    void Set_Viewed_Value();

    const std::string &getMTextMessage() const;
};