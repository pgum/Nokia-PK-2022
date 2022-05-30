#include "SmsDb.hpp"

ue::SmsDb::SmsDb() {
//    common::PhoneNumber number {000};
//    Sms sms {"SMS_TEST", number, number};
//    smsList.push_back(sms);
}

void ue::SmsDb::addSms(const std::string& text, common::PhoneNumber fromPhoneNumber, common::PhoneNumber toPhoneNumber) {
    Sms sms = Sms(text, fromPhoneNumber, toPhoneNumber);
    smsList.push_back(sms);
}
std::vector<Sms> ue::SmsDb::getSmsList() {
    return smsList;
}

std::optional<Sms> ue::SmsDb::retrieveSms(int index) {
    if(smsList.size() > index) {
        smsList[index].markAsRead();
        return smsList[index];
    }
    return std::nullopt;
}

void ue::SmsDb::markLastSmsSentAsFailed() {
    if(!smsList.empty()) {
        smsList.back().markAsNotReceived();
    }
}

bool ue::SmsDb::checkForNewSms()
{
    for(auto&& sms : smsList)
    {
        if(!sms.isRead())
        {
            return true;
        }
    }
    return false;
}
