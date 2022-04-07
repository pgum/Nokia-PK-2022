#pragma once
#include <string>
#include <ctime>
#include <chrono>
#include "PhoneNumber.hpp"



namespace ue
{
    class SMS{
        public:
            SMS(std::string smsMessage, common::PhoneNumber senderPhoneNumber);
            void changeViewStatus();

            int smsId;
        private:
            bool viewStatus;
            std::string smsMessage;
            common::PhoneNumber senderPhoneNumber;
            std::chrono::time_point <std::chrono::system_clock> recivingTime;
    };
    
}