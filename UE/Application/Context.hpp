#pragma once

#include "IEventsHandler.hpp"
#include "Logger/ILogger.hpp"
#include <memory>
#include "SMSDB/ISMSDatabase.hpp"

namespace ue
{

struct CallingStatus
{
    common::PhoneNumber callingNumber;
    bool isOutgoingCall;
};

struct Context
{
    common::ILogger& logger;
    IBtsPort& bts;
    IUserPort& user;
    ITimerPort& timer;
    ISMSDatabase& smsDb;
    CallingStatus currentCallingStatus {};
    std::unique_ptr<IEventsHandler> state{};

    template <typename State, typename ...Arg>
    void setState(Arg&& ...arg)
    {
        state = std::make_unique<State>(*this, std::forward<Arg>(arg)...);
    }
};



}
