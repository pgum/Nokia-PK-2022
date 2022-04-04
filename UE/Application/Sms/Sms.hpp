#pragma once
#include <string>
#include <ctime>
#include "PhoneNumber.hpp"



namespace ue
{
    class SMS{
        public:
            SMS();
            void changeViewStatus();
            void setSms();
            ~SMS();

            int smsId;
        private:
            bool viewStatus;
            std::string smsMessage;
            common::PhoneNumber senderPhoneNumber;
            time_t timeRecivingSms;
    };
}