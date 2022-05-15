#pragma once

#include "SMSDB/ISMSDatabase.hpp"
#include "SMSDB/ITextMessege.hpp"
#include "Constants/MenuConstans.h"
#include "IUeGui.hpp"
#include <memory>

namespace ue
{

class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;
};

class IUserPort
{
public:
    typedef const std::vector<std::pair<unsigned int, std::shared_ptr<ITextMessage>>> smsContainer;
    virtual ~IUserPort() = default;

    virtual void showNotConnected() = 0;
    virtual void showConnecting() = 0;

    virtual common::PhoneNumber getPhoneNumber() = 0;

    virtual void showMainMenu() = 0;
    virtual void showSMSList(const smsContainer&& smsList) = 0;
    virtual void showSMSList(const smsContainer& smsList) = 0;
    virtual void showSMS(ITextMessage& sms) = 0;
    virtual void showSMS(ITextMessage&& sms) = 0;

    virtual void showSMSNotification() = 0;

    virtual IUeGui::ISmsComposeMode& initSmsComposer() = 0;
    virtual IUeGui::IListViewMode& initListViewMode() = 0;
    virtual IUeGui::ITextMode& initTextMode() = 0;

    virtual void setAcceptCallback(const IUeGui::Callback& callback) = 0;
    virtual void setRejectCallback(const IUeGui::Callback& callback) = 0;
    virtual void setHomeCallback(const IUeGui::Callback& callback) = 0;

    virtual PhoneNumber getInputPhoneNumber(IUeGui::ISmsComposeMode &composer) = 0;
    virtual std::string getInputString(IUeGui::ISmsComposeMode &composer) = 0;

};

}
