#include "SMS_DB.hpp"

SMS_DB::SMS_DB() {};

void SMS_DB::addSmsToDB(SMS sms) {
    this->m_smsList.push_back(sms);
}

std::vector<SMS> SMS_DB::getAllSMS() {
    return m_smsList;
}

SMS SMS_DB::getSingleSmsAt(int smsListPosition){
    m_smsList[smsListPosition].Set_Viewed_Value();
    return this->m_smsList[smsListPosition];
}
