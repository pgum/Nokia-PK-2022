#pragma once
#include <string>
#include <vector>
#include "Sms.hpp"
#include "PhoneNumber.hpp"

namespace ue
{

class SmsDb
{
public:
    SmsDb();
    ~SmsDb();
    void receiveSms(); //uploading sms from bts
    void viewSms(bool state); //to see concrete sms or list of sms, both state
    void sendSms(common::PhoneNumber reciverPhoneNumber); // to make new object in smsList and pass to bts


private:
    std::vector <SMS> smsList;
};
}