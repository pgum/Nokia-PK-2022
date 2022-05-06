#pragma once

#include <Models/SmsDb.hpp>
#include <IUeGui.hpp>

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
    virtual ~IUserPort() = default;

    virtual void showNotConnected() = 0;
    virtual void showConnecting() = 0;
    virtual void showConnected() = 0;

    virtual void showSmsList() = 0;
    virtual void showSms(int index) = 0;
    virtual int getAction() = 0;
    virtual void acceptCallback(IUeGui::Callback acceptCallback) = 0;
    virtual void rejectCallback(IUeGui::Callback rejectCallback) = 0;
    virtual SmsDb& getSmsDb() = 0;
};

}
