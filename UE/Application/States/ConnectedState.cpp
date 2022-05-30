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


void ConnectedState::handleDisconnected()
{
    context.setState<NotConnectedState>();
}

void ConnectedState::handleTimeout()
{
    context.bts.sendCallDropped(context.currentCallingStatus.callingNumber);
    context.currentCallingStatus.callingNumber.value = 0;
    context.currentCallingStatus.isOutgoingCall = false;
    handleMainMenu();
}


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
    using namespace std::chrono_literals;
    context.timer.startTimer(60000ms);

    auto phoneNumber = dial.getPhoneNumber();
    context.currentCallingStatus.callingNumber = phoneNumber;
    context.currentCallingStatus.isOutgoingCall = true;
    context.user.showDialing(phoneNumber);
    context.bts.sendCallRequest(phoneNumber);
    context.user.setAcceptCallback([&]{return; });
    context.user.setRejectCallback([&]{handleCallResignation();});
}

void ConnectedState::handleCallRequest(common::PhoneNumber from)
{
    using namespace std::chrono_literals;
    if(context.currentCallingStatus.isOutgoingCall){
        context.bts.sendCallDropped(context.currentCallingStatus.callingNumber);
        context.currentCallingStatus.callingNumber.value = 0;
        context.currentCallingStatus.isOutgoingCall = false;
    }
    if(context.currentCallingStatus.callingNumber.value == 0)
    {
        context.timer.startTimer(30000ms);
        context.currentCallingStatus.callingNumber = from;
        context.currentCallingStatus.isOutgoingCall = false;
        context.user.showNewCallRequest(from);
        context.user.setAcceptCallback([&]{handleAcceptOnCallRequest(); });
        context.user.setRejectCallback([&]{handleRejectOnCallRequest();});
    } else {
        context.bts.sendCallDropped(from);
    }
}

void ConnectedState::handleCallAccepted(common::PhoneNumber)
{
    context.timer.stopTimer();

    context.user.showTalking();
    context.setState<TalkingState>();
}

void ConnectedState::handleCallDropped(common::PhoneNumber)
{
    context.timer.stopTimer();

    context.currentCallingStatus.callingNumber.value = 0;
    context.currentCallingStatus.isOutgoingCall = false;
    context.user.showCallDropped();
    context.user.setAcceptCallback([&]{ handleMainMenu(); });
    context.user.setRejectCallback([&]{ handleMainMenu(); });

}

void ConnectedState::handleAcceptOnCallRequest()
{
    context.timer.stopTimer();

    context.bts.sendCallAccepted(context.currentCallingStatus.callingNumber);
    context.user.showTalking();
    context.setState<TalkingState>();
}

void ConnectedState::handleRejectOnCallRequest()
{
    context.timer.stopTimer();

    context.bts.sendCallDropped(context.currentCallingStatus.callingNumber);
    context.currentCallingStatus.callingNumber.value = 0;
    context.currentCallingStatus.isOutgoingCall = false;
    handleMainMenu();
}

void ConnectedState::handleCallResignation()
{
    context.timer.stopTimer();
    context.bts.sendCallDropped(context.currentCallingStatus.callingNumber);
    context.currentCallingStatus.callingNumber.value = 0;
    context.currentCallingStatus.isOutgoingCall = false;
    handleMainMenu();
}

void ConnectedState::handleUnknownRecipientAfterCallRequest()
{
    context.timer.stopTimer();

    context.user.showPartnerNotAvailable();
    context.user.setAcceptCallback([&]{ handleMainMenu(); });
    context.user.setRejectCallback([&]{ handleMainMenu(); });
}

void ConnectedState::startDial()
{
    IUeGui::IDialMode& dial = context.user.initDialMode();
    context.user.setAcceptCallback([&]{handleAcceptOnDial(dial); });
    context.user.setRejectCallback([&]{ handleMainMenu(); });
}



}
