#include "SMS_DB.hpp"

SMS_DB::SMS_DB(){};

SMS_DB::~SMS_DB(){};

void SMS_DB::addSmsToDB(SMS sms)
{
    this->m_smsList.push_back(sms);
}

void SMS_DB::getAllSMS()
{
    //TODO
}

