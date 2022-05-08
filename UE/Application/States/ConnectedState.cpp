#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "ViewingSmsState.hpp"
#include "SendingSmsState.hpp"



namespace ue
{



    ConnectedState::ConnectedState(Context &context)
            : BaseState(context, "ConnectedState")
    {
        context.user.acceptCallback([this] { onAcceptCallbackClicked(); });
        context.user.rejectCallback([this] { onDeclineCallbackClicked(); });
        context.user.showConnected();
    }



    void ConnectedState::onAcceptCallbackClicked()
    {
        int currentMenuIndex = context.user.getMenuIndex();



        if (currentMenuIndex == 1)
        {
            context.setState<SendingSmsState>();
        }
        if (currentMenuIndex == 2)
        {
            context.setState<ViewingSmsState>();
        }
    }



    void ConnectedState::onDeclineCallbackClicked()
    {
        context.logger.logInfo("Jakub Pietrzko");
    }



    void ConnectedState::handleDisconnected()
    {
        context.setState<NotConnectedState>();
    }



    void ConnectedState::handleSMSReceive(const std::string smsText, const common::PhoneNumber senderNumber) {
        SMS sms1 = SMS(smsText, senderNumber);
        SMS_DB ourDataBase = context.user.getSmsDB();
        ourDataBase.addSmsToDB(sms1);
        context.user.setSmsDB(ourDataBase);
    }



}