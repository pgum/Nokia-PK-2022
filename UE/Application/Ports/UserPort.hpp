#pragma once

#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "IUeGui.hpp"
#include "Messages/PhoneNumber.hpp"
#include "../SMS/SMS_DB.hpp"

namespace ue {

    class UserPort : public IUserPort {
    public:
        UserPort(common::ILogger &logger, IUeGui &gui, common::PhoneNumber phoneNumber);

        void start(IUserEventsHandler &handler);

        void stop();

        void showNotConnected() override;

        void showConnecting() override;

        void showConnected(int notification) override;

        void showViewingSms() override;

        SMS returnSingleSms(int smsListPosition) override;

        void showSingleSms(int smsListPosition) override;

        IUeGui::ISmsComposeMode &composeSmsMode() override;

        IUeGui::IDialMode &dialComposeMode() override;

        IUeGui::ITextMode &callFromMode(const common::PhoneNumber callerNumber) override;

        IUeGui::ITextMode &callToMode(const common::PhoneNumber receiverNumber) override;

        void acceptCallbackClicked(IUeGui::IListViewMode &menu);

        void acceptCallback(IUeGui::Callback acceptCallback) override;

        void rejectCallback(IUeGui::Callback rejectCallback) override;

        int getMenuIndex() override;

        SMS_DB &getSmsDB() override;

        SMS_DB smsDb;

        void setSmsDB(SMS_DB &smsDb) override;

    private:
        common::PrefixedLogger logger;
        IUeGui &gui;
        common::PhoneNumber phoneNumber;
        IUserEventsHandler *handler = nullptr;
        IUeGui::Callback currentCallbackState;
        int menuIndex = -1;

    };

}
