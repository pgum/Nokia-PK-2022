#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "ViewingSmsState.hpp"
#include "SendingSmsState.hpp"
#include "SendingCallRequest.hpp"
#include "ReceivingCallRequest.hpp"


namespace ue {
    ConnectedState::ConnectedState(Context &context, int notification)
            : BaseState(context, "ConnectedState") {
        this->notification = notification;
        context.user.acceptCallback([this] { onAcceptCallbackClicked(); });
        context.user.rejectCallback([this] { onDeclineCallbackClicked(); });
        context.user.showConnected(this->notification);
    }

    void ConnectedState::onAcceptCallbackClicked() {
        int currentMenuIndex = context.user.getMenuIndex();

        if (currentMenuIndex == 0) {
            context.setState<SendingSmsState>(this->notification);
        }
        if (currentMenuIndex == 1) {
            this->notification = 0;
            context.setState<ViewingSmsState>();
        }
        if (currentMenuIndex == 2) {
            context.setState<SendingCallRequestState>();
        }
    }

    void ConnectedState::onDeclineCallbackClicked() {}

    void ConnectedState::handleDisconnected() {
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

    void ConnectedState::handleCallRequest(const common::PhoneNumber callerNumber) {
        context.setState<ReceivingCallRequest>(callerNumber);
    }

}