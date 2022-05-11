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
    IUeGui& guiRef;

    void handleMainMenu();
    void handleSMSList();
    void handleComposeSMSView();

    void handleAcceptOnMainMenu();
    void handleAcceptOnSMSList();
    void handleSMSView(unsigned int elemIndex);
    void handleAcceptOnComposeSMSView(IUeGui::ISmsComposeMode& smsComposer);


public:
    ConnectedState(Context& context);
    void handleDisconnected() override;

};

}
