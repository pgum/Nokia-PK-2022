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
    if(kindOfSms=="sended"){
        m_isSended = true;
    }else if(kindOfSms == "received"){
       m_isSended = false;
    }
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
    std::string kindOfSms;
    std::string status;

    if(this->m_isViewed){
        kindOfSms = "P: ";
    }else if(!this->m_isViewed)
        kindOfSms = "NP: ";

    if(this->m_isSended){
        status = "W ";
    }else if(!this->m_isSended)
        status = "O ";

    return status + kindOfSms + common::to_string(m_sender)+" "+m_textMessage.substr(0,5)+"...";

}

std::string SMS::isMIsViewed() const{
    if(this->m_isViewed){
        return "true";
    }
    return "false";
}
