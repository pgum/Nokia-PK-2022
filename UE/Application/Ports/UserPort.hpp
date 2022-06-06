#pragma once

#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "IUeGui.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Models/SmsDb.hpp"
#include "Models/Sms.hpp"

namespace {
    int NO_ACTION = -1;
}

namespace ue {

    class UserPort : public IUserPort {
    public:
        UserPort(common::ILogger &logger, IUeGui &gui, common::PhoneNumber phoneNumber);

        void start(IUserEventsHandler &handler);

        void stop();

        void showNotConnected() override;

        void showConnecting() override;

        void showConnected() override;

        void showSms(int index) override;

        void showSmsList() override;

        IUeGui::ISmsComposeMode &composeSms() override;

        int getAction() override;

        void acceptCallback(IUeGui::Callback acceptCallback) override;

        void rejectCallback(IUeGui::Callback rejectCallback) override;

        SmsDb &getSmsDb() override;

        void showCallRequest(common::PhoneNumber) override;

        void callAchieved(common::PhoneNumber) override;

        void startTalking(common::PhoneNumber) override;

        void showPartnerNotAvailable(common::PhoneNumber) override;

        void showEnterPhoneNumber() override;

        void showDialing(common::PhoneNumber) override;

    private:
        common::PrefixedLogger logger;
        IUeGui &gui;
        IUeGui::Callback callback;
        common::PhoneNumber phoneNumber;
        IUserEventsHandler *handler = nullptr;
        SmsDb smsDb;
        int action = NO_ACTION;

        void onAcceptCallback(IUeGui::IListViewMode &menu);

        std::optional <Sms> viewSms(int index);

        std::vector <Sms> viewSmsList();
    };

}
