#pragma once

#include "Messages/PhoneNumber.hpp"
#include "IUeGui.hpp"

namespace ue
{

class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;

    virtual void handleSendCallAccept(common::PhoneNumber) = 0;
    virtual void handleSendCallReject(common::PhoneNumber) = 0;
};

class IUserPort
{
public:
    virtual ~IUserPort() = default;

    virtual void showNotConnected() = 0;
    virtual void showConnecting() = 0;
    virtual void showConnected() = 0;

    virtual void showNewSMS() = 0;

    virtual void showNewCallRequest(common::PhoneNumber from) = 0;
};

}
