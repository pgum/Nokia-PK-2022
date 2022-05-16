#pragma once

#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "IUeGui.hpp"
#include "UeGui/ITextMode.hpp"
#include "UeGui/ISmsComposeMode.hpp"
#include "UeGui/IListViewMode.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Constants/MenuConstans.h"

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

    common::PhoneNumber getPhoneNumber() override;

    void showMainMenu() override;

    void showSMSList(const smsContainer&& smsList) override;
    void showSMSList(const smsContainer& smsList) override;

    void showSMS(ITextMessage& sms) override;
    void showSMS(ITextMessage&& sms) override;

    void showSMSNotification() override;

    IUeGui::ISmsComposeMode& initSmsComposer() override;
    IUeGui::IListViewMode& initListViewMode() override;
    IUeGui::ITextMode& initTextMode() override;

    void setAcceptCallback(const IUeGui::Callback& callback) override;
    void setRejectCallback(const IUeGui::Callback& callback) override;
    void setHomeCallback(const IUeGui::Callback& callback) override;

    PhoneNumber getInputPhoneNumber(IUeGui::ISmsComposeMode &composer) override;
    std::string getInputString(IUeGui::ISmsComposeMode &composer) override;

private:
    common::PrefixedLogger logger;
    IUeGui& gui;
    common::PhoneNumber phoneNumber;
    IUserEventsHandler* handler = nullptr;
};

}
