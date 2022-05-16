#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "ViewingSmsState.hpp"
#include "SendingSmsState.hpp"



namespace ue
{



    ConnectedState::ConnectedState(Context &context, int powiadomienie)
            : BaseState(context, "ConnectedState")
    {
        this->notification = powiadomienie;
        context.user.acceptCallback([this] { onAcceptCallbackClicked(); });
        context.user.rejectCallback([this] { onDeclineCallbackClicked(); });
        context.user.showConnected(this->notification);
    }



    void ConnectedState::onAcceptCallbackClicked()
    {
        int currentMenuIndex = context.user.getMenuIndex();



        if (currentMenuIndex == 1)
        {
            context.setState<SendingSmsState>(this->notification);
        }
        if (currentMenuIndex == 2)
        {
            this->notification=0;
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
        this->notification = 1;
        SMS sms1 = SMS(smsText, senderNumber, "received");
        SMS_DB ourDataBase = context.user.getSmsDB();
        ourDataBase.addSmsToDB(sms1);
        context.user.setSmsDB(ourDataBase);
        context.user.showConnected(this->notification);
    }

}