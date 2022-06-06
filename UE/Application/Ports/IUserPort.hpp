#pragma once

#include <Models/SmsDb.hpp>
#include <IUeGui.hpp>

namespace ue {

    class IUserEventsHandler {
    public:
        virtual ~IUserEventsHandler() = default;

        virtual void handleCallAccept(common::PhoneNumber)=0;
        virtual void handleStartDial()=0;
        virtual void handleCallRequest(common::PhoneNumber)=0;
        virtual void handleCallDrop(common::PhoneNumber) =0;
    };

    class IUserPort {
    public:
        virtual ~IUserPort() = default;

        virtual void showNotConnected() = 0;

        virtual void showConnecting() = 0;

        virtual void showConnected() = 0;

        virtual void showSmsList() = 0;

        virtual void showSms(int index) = 0;

        virtual IUeGui::ISmsComposeMode &composeSms() = 0;

        virtual int getAction() = 0;

        virtual void acceptCallback(IUeGui::Callback acceptCallback) = 0;

        virtual void rejectCallback(IUeGui::Callback rejectCallback) = 0;

        virtual SmsDb &getSmsDb() = 0;

        virtual void showCallRequest(common::PhoneNumber) = 0;

        virtual void callAchieved(common::PhoneNumber) = 0;

        virtual void startTalking(common::PhoneNumber) = 0;

        virtual void showPartnerNotAvailable(common::PhoneNumber) = 0;

        virtual void showEnterPhoneNumber() = 0;

        virtual void showDialing(common::PhoneNumber) = 0;

    };

}
