#include <Sms.hpp>


ue::SMS::SMS( std::string set_smsMessage, common::PhoneNumber set_senderPhoneNumber)
{
    ue::SMS::viewStatus = 1; //unread
    ue::SMS::smsMessage = set_smsMessage;
    ue::SMS::senderPhoneNumber = set_senderPhoneNumber;
    ue::SMS::smsId ;
    ue::SMS::recivingTime = std::chrono::system_clock::now();

}
void ue::SMS::changeViewStatus()
{
    //todo
    //ustawienie ze dany sms jest lub by odczytany
}
