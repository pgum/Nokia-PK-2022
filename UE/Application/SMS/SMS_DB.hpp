#include "SMS.hpp"
#include <vector>
#pragma once

class SMS_DB{
    std::vector<SMS> m_smsList;

public:
    SMS_DB();
    void addSmsToDB(SMS sms);

    void getAllSMS();

    virtual ~SMS_DB();
};
