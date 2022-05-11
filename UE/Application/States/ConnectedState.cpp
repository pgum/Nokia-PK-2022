#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"

namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState"),
    guiRef(context.user.getUserGui())
{
    handleMainMenu();
}

void ConnectedState::handleDisconnected()
{
    context.setState<NotConnectedState>();
}





#pragma region MainMenu
void ConnectedState::handleMainMenu()
{
    context.user.showMainMenu();
    guiRef.setAcceptCallback([&]{ handleAcceptOnMainMenu(); });
    guiRef.setRejectCallback([&]{ return; });
}


void ConnectedState::handleAcceptOnMainMenu()
{
    IUeGui::IListViewMode& mainMenu = context.user.initListViewMode();
    auto[isElementSelected,elemIndex] = mainMenu.getCurrentItemIndex();
    if(not isElementSelected)
        return;

    mainMenu.clearSelectionList();
    switch (elemIndex)
    {
        case VIEW_SMS_LIST:
        {
            handleSMSList();
            break;
        }
        case COMPOSE_SMS:
        {
            handleComposeSMSView();
            break;
        }
        default:
            return;
    }

}
#pragma endregion





#pragma region SMSList
void ConnectedState::handleSMSList()
{
    context.user.showSMSList(context.smsDb.getAllSMS());
    guiRef.setAcceptCallback([&]{ handleAcceptOnSMSList(); });
    guiRef.setRejectCallback([&]{ handleMainMenu(); });
}


void ConnectedState::handleAcceptOnSMSList()
{
    IUeGui::IListViewMode& mainMenu = context.user.initListViewMode();
    auto[isElementSelected,elemIndex] = mainMenu.getCurrentItemIndex();
    if(not isElementSelected)
        return;

    handleSMSView(elemIndex);

}

#pragma endregion




#pragma region SMSView
void ConnectedState::handleSMSView(unsigned int elemIndex)
{
    context.user.showSMS(context.smsDb.getSMS(elemIndex));
    guiRef.setAcceptCallback([&]{ return; });
    guiRef.setRejectCallback([&]{ handleSMSList(); });
}

void ConnectedState::handleComposeSMSView()
{
    IUeGui::ISmsComposeMode& smsComposer = context.user.initSmsComposer();
    smsComposer.clearSmsText();

    guiRef.setAcceptCallback([&]{ handleAcceptOnComposeSMSView(smsComposer); });
    guiRef.setRejectCallback([&]{ handleMainMenu(); });
}

void ConnectedState::handleAcceptOnComposeSMSView(IUeGui::ISmsComposeMode& smsComposer)
{
    context.smsDb.addSMS(
            context.user.getInputPhoneNumber(smsComposer),
            context.user.getPhoneNumber(),
            context.user.getInputString(smsComposer));

    handleMainMenu();
}

#pragma endregion


}
