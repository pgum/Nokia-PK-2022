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
    m_isViewed = false;
}

SMS::SMS(const std::string &mTextMessage, const common::PhoneNumber &mSender, const std::string kindOfSms) {
    m_textMessage = mTextMessage;
    m_sender = mSender;
    m_isViewed = false;
    if(kindOfSms=="send"){
        m_isSend = true;
    }else if(kindOfSms == "received"){
        m_isSend = false;
    }
}

void SMS::Set_Viewed_Value() {
    this->m_isViewed = true;
}

const std::string &SMS::getMTextMessage() const {
    return m_textMessage;
}

const std::string SMS::getSmsHeader(){
    std::string kindOfSms;
    std::string status;
    kindOfSms = "";
    if(!this->m_isViewed)kindOfSms = "[N]";
    if(this->m_isSend){
        kindOfSms = "";
        status = "To:";
    }else if(!this->m_isSend)
        status = "From:";

    return kindOfSms + status + common::to_string(m_sender)+"|"+m_textMessage.substr(0,4)+"...";
}
