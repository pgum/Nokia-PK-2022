#pragma once
#include <string>
#include <vector>
#include "Sms.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class SmsDb
{
public:
    SmsDb() = default;
    ~SmsDb() = default;

    void addSmsToDB(Sms newSms);
    Sms getSmsFromDB(int smsIndex);
    std::vector<Sms>& getSmsList();

private:
    std::vector <Sms> smsList;
};
}