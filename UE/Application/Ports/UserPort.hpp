#pragma once

#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "IUeGui.hpp"
#include "UeGui/ITextMode.hpp"
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
    void showConnected(ISMSDatabase& smsDb) override;
    void showNewSMS() override;
    void showSMSList(ISMSDatabase& smsDb) override;
private:
    common::PrefixedLogger logger;
    IUeGui& gui;
    common::PhoneNumber phoneNumber;
    IUserEventsHandler* handler = nullptr;


    void handleMainMenuInput(ISMSDatabase &smsDb);
    void showSelectedSMS(ISMSDatabase &smsDb);
};

}
