#pragma once

#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "IUeGui.hpp"
#include "Messages/PhoneNumber.hpp"
#include "../SMS/SMS_DB.hpp"

namespace ue
{

class UserPort : public IUserPort
{
public:
    UserPort(common::ILogger& logger, IUeGui& gui, common::PhoneNumber phoneNumber);
    void start(IUserEventsHandler& handler);
    void stop();

    void showNotConnected() override;
    void showConnecting() override;
    void showConnected() override;
    void showViewingSms() override;
    IUeGui::ISmsComposeMode& composeSmsMode() override;
    void acceptCallbackClicked(IUeGui::IListViewMode& menu);
    void acceptCallback(IUeGui::Callback acceptCallback) override;
    void rejectCallback(IUeGui::Callback rejectCallback) override;
    int getMenuIndex() override;
    SMS_DB &getSmsDB() override;
    SMS_DB smsDb;
    void setSmsDB(SMS_DB &smsDb) override;

private:
    common::PrefixedLogger logger;
    IUeGui& gui;
    common::PhoneNumber phoneNumber;
    IUserEventsHandler* handler = nullptr;
    IUeGui::Callback currentCallbackState;
    int menuIndex = -1;

};

}
