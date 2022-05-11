#include "Sms.hpp"
namespace ue {

    Sms::Sms(common::PhoneNumber from, common::PhoneNumber to, std::string smsMessage) {
        Sms::from = from;
        Sms::to = to;
        Sms::flgIsViewed = false;
    }

    void Sms::setSmsIsViewed() {
        flgIsViewed = true;
    }

    common::PhoneNumber Sms::getFrom() {
        return from;
    }

    common::PhoneNumber Sms::getTo() {
        return to;
    }

    std::string Sms::getsmsMessageTest() {
        return smsMessage;
    }

    bool Sms::isViewed() {
        return flgIsViewed;
    }
}

