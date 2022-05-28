#pragma once

#include "IUeGui.hpp"
#include "SMS/SMS_DB.hpp"

namespace ue {

    class IUserEventsHandler {
    public:
        virtual ~IUserEventsHandler() = default;
    };

    class IUserPort {
    public:
        virtual ~IUserPort() = default;

        virtual void showNotConnected() = 0;

        virtual void showConnecting() = 0;

        virtual void showConnected(int notification) = 0;

        virtual int getMenuIndex() = 0;

        virtual void showViewingSms() = 0;

        virtual SMS returnSingleSms(int smsListPosition) = 0;

        virtual void showSingleSms(int smsListPosition) = 0;

        virtual IUeGui::ISmsComposeMode& composeSmsMode() = 0;

        virtual IUeGui::IDialMode& dialComposeMode() = 0;

        virtual IUeGui::ITextMode& callFromMode(const common::PhoneNumber callerNumber) = 0;

        virtual IUeGui::ITextMode& callToMode(const common::PhoneNumber receiverNumber) = 0;

        virtual void acceptCallback(IUeGui::Callback acceptCallback) = 0;

        virtual void rejectCallback(IUeGui::Callback rejectCallback) = 0;

        virtual SMS_DB& getSmsDB() = 0;
        virtual void setSmsDB(SMS_DB &smsDb) = 0;
    };

}
