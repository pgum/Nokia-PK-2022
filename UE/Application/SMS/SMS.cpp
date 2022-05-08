#include "SMS.hpp"

SMS::SMS(common::MessageId mMessageId, const common::PhoneNumber &mSender, const common::PhoneNumber &mReceiver,
         const std::string &mTextMessage) :
        m_messageID(mMessageId),
        m_sender(mSender),
        m_receiver(mReceiver),
        m_textMessage(mTextMessage),
        m_isViewed(false) {}

SMS::SMS(const std::string &mTextMessage, const common::PhoneNumber &mSender) {
    m_textMessage = mTextMessage;
    m_sender = mSender;
}

void SMS::Set_Viewed_Value() {
    this->m_isViewed = true;
}

const common::PhoneNumber &SMS::getMSender() const {
    return m_sender;
}

const std::string &SMS::getMTextMessage() const {
    return m_textMessage;
}

const std::string SMS::getSmsHeader(){
    return common::to_string(m_sender)+" "+m_textMessage;
}
