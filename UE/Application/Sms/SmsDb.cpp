#pragma once
#include "PhoneNumber.hpp"
#include "SmsDb.hpp"


ue::SmsDb::SmsDb()
{
    //TODO:
    // stworyć konstruktor
}
ue::SmsDb::~SmsDb()
{
    //TODO:
    //Stworzyć dekostrukor

}
void ue::SmsDb::receiveSms() //uploading sms from bts
{
    //TODO
    //uruchomienie odpowiedznich poleceń i zapisanie wiadomości do listy sms
}
void ue::SmsDb::viewSms(bool state) //to see concrete sms or list of sms, both state
{
    //TODO
    //na podstawie stanu uruchomienie listy wiadomości lub wejscie w konkretną wiadomość
}
void ue::SmsDb::sendSms(common::PhoneNumber reciverPhoneNumber) // to make new object in smsList and pass to bts
{
    //TODO:
    //wysłanie na konkretny numer wiadomości
}