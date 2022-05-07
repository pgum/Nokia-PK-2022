#pragma once

#include "SMSDB/ISMSDatabase.hpp"

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
    virtual void showConnected(const ISMSDatabase& smsDb) = 0;

    virtual void showNewSMS() = 0;
    virtual void showSMSList() = 0;
};

}
