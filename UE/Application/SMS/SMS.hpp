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
    bool m_isSend;

public:
    SMS(common::MessageId mMessageId, const common::PhoneNumber &mSender, const common::PhoneNumber &mReceiver,
        const std::string &mTextMessage);
    SMS(const std::string &mTextMessage, const common::PhoneNumber &mSender);
    SMS(const std::string &mTextMessage, const common::PhoneNumber &mSender, const std::string kindOfSms);

    void Set_Viewed_Value();

    const std::string &getMTextMessage() const;

    const std::string getSmsHeader();
};
