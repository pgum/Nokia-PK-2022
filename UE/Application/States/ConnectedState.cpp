#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "TalkingState.hpp"

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

void ConnectedState::handleCallRequest(common::PhoneNumber from)
{
    using namespace std::chrono_literals;
    context.user.showNewCallRequest(from);
    context.timer.startTimer(30000ms);
    context.user.setAcceptCallback([&]{handleSendCallAccept(from); });
    context.user.setRejectCallback([&]{handleSendCallReject(from);});
}

void ConnectedState::handleSendCallAccept(common::PhoneNumber to)
{
    context.bts.sendCallAccept(to);
    context.timer.stopTimer();
    context.setState<TalkingState>();
    context.user.showTalking();

    //TODO Dodanie logiki dla przerywanie połączenia
}

void ConnectedState::handleSendCallReject(common::PhoneNumber to)
{
    context.bts.sendCallReject(to);
    context.timer.stopTimer();
    handleMainMenu();
}

void ConnectedState::handleTimeout()
{
    context.user.showMainMenu();
    //TODO Metoda powinna jeszcze wysyłać sendCallReject(to), więc trzeba gdzieś zapisywać numer dzwoniącego
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
        case DIAL:
        {
            startDial();
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

void ConnectedState::handleAcceptOnDial(IUeGui::IDialMode& dial)
{
    auto phoneNumber = dial.getPhoneNumber();
    context.bts.sendCallRequest(phoneNumber);
}

void ConnectedState::startDial()
{
    IUeGui::IDialMode& dial = context.user.initDialMode();
    context.user.setAcceptCallback([&]{
        handleAcceptOnDial(dial); });
    context.user.setRejectCallback([&]{ handleMainMenu(); });
}


}
