#include "SmsDb.hpp"

ue::SmsDb::SmsDb() {
    smsList.push_back(Sms());
}

void ue::SmsDb::addSms(std::string text) {

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