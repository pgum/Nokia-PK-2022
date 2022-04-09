#include "SMS.hpp"

SMS::SMS(common::MessageId mMessageId, const common::PhoneNumber &mSender, const common::PhoneNumber &mReceiver, const std::string &mTextMessage) :
         m_messageID(mMessageId),
         m_sender(mSender),
         m_receiver(mReceiver),
         m_textMessage(mTextMessage),
         m_isViewed(false){}

void SMS::Set_Viewed_Value() {
    this->m_isViewed = true;
}



