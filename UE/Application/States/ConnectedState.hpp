#pragma once

#include "BaseState.hpp"
#include "Constants/MenuConstans.h"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ISmsComposeMode.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
private:

    void handleMainMenu();
    void handleAcceptOnMainMenu();

    void handleTimeout() override;

    void handleSMSList();
    void handleComposeSMSView();
    void handleAcceptOnSMSList();
    void handleSMSView(unsigned int elemIndex);
    void handleAcceptOnComposeSMSView(IUeGui::ISmsComposeMode& smsComposer);
    void handleSMS(common::PhoneNumber from, std::string text, common::MessageId msgType) override;

    void startDial();
    void handleAcceptOnDial(IUeGui::IDialMode&);

    void handleAcceptOnCallRequest();
    void handleRejectOnCallRequest();

    void handleCallResignation();

public:
    ConnectedState(Context& context);
    void handleDisconnected() override;

    void handleCallRequest(common::PhoneNumber) override;
    void handleCallAccepted(common::PhoneNumber) override;
    void handleCallDropped(common::PhoneNumber) override;
    void handleUnknownRecipientAfterCallRequest() override;


};

}
