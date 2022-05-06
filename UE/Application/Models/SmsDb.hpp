#pragma once
#include <Models/Sms.hpp>
#include <vector>
#include <string>
#include <optional>

namespace ue {

    class SmsDb {

    private:
        std::vector<Sms> smsList;
    public:
        SmsDb();
        void addSms(std::string);
        std::vector<Sms> getSmsList();
        std::optional<Sms> retrieveSms(int index);
    };
}