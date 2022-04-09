#include "SMS.hpp"
#include <vector>
#pragma once

class SMS_DB{
    std::vector<SMS> m_smsList;

public:
    void addSmsToDB(SMS sms);

    void getAllSMS();

};
