#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"

namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
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
    context.user.setAcceptCallback([&]{ handleAcceptOnMainMenu(); });
    context.user.setRejectCallback([&]{ return; });
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
    context.user.setAcceptCallback([&]{ handleAcceptOnSMSList(); });
    context.user.setRejectCallback([&]{ handleMainMenu(); });
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
    context.user.showSMS( context.smsDb.getSMS(elemIndex));
    context.user.setAcceptCallback([&]{ return; });
    context.user.setRejectCallback([&]{ handleSMSList(); });
}

void ConnectedState::handleComposeSMSView()
{
    IUeGui::ISmsComposeMode& smsComposer = context.user.initSmsComposer();
    smsComposer.clearSmsText();

    context.user.setAcceptCallback([&]{ handleAcceptOnComposeSMSView(smsComposer); });
    context.user.setRejectCallback([&]{ handleMainMenu(); });
}

void ConnectedState::handleAcceptOnComposeSMSView(IUeGui::ISmsComposeMode& smsComposer)
{
    auto receiverNumber  = context.user.getInputPhoneNumber(smsComposer);
    auto msgToSend = context.user.getInputString(smsComposer);

    context.smsDb.addSMS(
            context.user.getPhoneNumber(),
            receiverNumber ,
            msgToSend,
            true,
            Send);

    context.bts.sendSMS(receiverNumber,msgToSend);
    handleMainMenu();
}


#pragma endregion


#pragma region smsHandling
void ConnectedState::handleSMS(common::PhoneNumber from, std::string text, common::MessageId msgType)
{
    switch(msgType) {
        case common::MessageId::Sms:
        {
            context.smsDb.addSMS(
                    from,
                    context.user.getPhoneNumber(),
                    text,
                    false,
                    Received
            );
            context.user.showSMSNotification();
            break;
        }
        case common::MessageId::UnknownRecipient:
        {
            auto sms = context.smsDb.getLastSMSSend();
            if(sms)
                sms.value()->setSMSTransmissionState(Bounce);
        }
    }
}
#pragma endregion

}
