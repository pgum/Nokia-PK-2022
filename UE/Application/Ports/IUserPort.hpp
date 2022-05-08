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

        virtual void showConnected() = 0;

        virtual int getMenuIndex() = 0;

        virtual void showViewingSms() = 0;

        virtual IUeGui::ISmsComposeMode& composeSmsMode() = 0;

        virtual void acceptCallback(IUeGui::Callback acceptCallback) = 0;

        virtual void rejectCallback(IUeGui::Callback rejectCallback) = 0;

        virtual SMS_DB& getSmsDB() = 0;
        virtual void setSmsDB(SMS_DB &smsDb) = 0;
    };

}