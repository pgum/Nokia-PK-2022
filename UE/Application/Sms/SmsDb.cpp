#include "SmsDb.hpp"


void ue::SmsDb::addSmsToDB(Sms newSms)
{
    auto it = smsList.begin();
    smsList.insert(it, newSms);
}
ue::Sms ue::SmsDb::getSmsFromDB(int smsId) 
{
    if(smsId >= smsList.size()) {
        return Sms({}, {}, "");
    } else {
        return smsList[smsId];
    }
}


std::vector<ue::Sms>& ue::SmsDb::getSmsList() {
    return smsList;
}