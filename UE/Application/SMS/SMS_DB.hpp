#include "SMS.hpp"
#include <vector>

#pragma once

class SMS_DB {
    std::vector<SMS> m_smsList;

public:
    SMS_DB();

    void addSmsToDB(SMS sms);

    std::vector<SMS> getAllSMS();

    SMS getSingleSmsAt(int smsListPosition);
};
