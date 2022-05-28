#include "ConnectedState.hpp"
#include "SendingSmsState.hpp"

namespace ue {

    SendingSmsState::SendingSmsState(Context &context, int notification) : BaseState(
            context, "SendingSmsState"), smsComposeMode(context.user.composeSmsMode()) {
        smsComposeMode.clearSmsText();
        this->notification = notification;
        context.user.acceptCallback([this] { onAcceptCallbackClicked(); });
        context.user.rejectCallback([this] { onDeclineCallbackClicked(); });
    }

    void SendingSmsState::onAcceptCallbackClicked() {
        sendSms();
        context.setState<ConnectedState>(this->notification);
    }

    void SendingSmsState::onDeclineCallbackClicked() {
        context.setState<ConnectedState>(this->notification);
    }

    void SendingSmsState::sendSms() {
        common::PhoneNumber receiverNumber = smsComposeMode.getPhoneNumber();
        std::string smsText = smsComposeMode.getSmsText();
        SMS sms = SMS(smsText, receiverNumber, "send");
        context.user.getSmsDB().addSmsToDB(sms);
        context.logger.logInfo(smsText);
        context.logger.logInfo(receiverNumber);
        context.bts.sendSms(receiverNumber, smsText);
    }

    void SendingSmsState::handleSMSReceive(const std::string smsText, const common::PhoneNumber senderNumber) {
        this->notification = 1;
        SMS sms = SMS(smsText, senderNumber, "received");
        SMS_DB ourDataBase = context.user.getSmsDB();
        ourDataBase.addSmsToDB(sms);
        context.user.setSmsDB(ourDataBase);
    }

}