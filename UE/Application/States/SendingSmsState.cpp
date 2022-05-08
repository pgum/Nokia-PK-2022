#include "ConnectedState.hpp"
#include "SendingSmsState.hpp"

namespace ue {

    SendingSmsState::SendingSmsState(Context &context) : ConnectedState(
            context), smsComposeMode(context.user.composeSmsMode()) {
        smsComposeMode.clearSmsText();
        context.user.acceptCallback([this] { onAcceptCallbackClicked(); });
        context.user.rejectCallback([this] { onDeclineCallbackClicked(); });
    }

    void SendingSmsState::onAcceptCallbackClicked()
    {
        sendSms();
        context.setState<ConnectedState>();
    }

    void SendingSmsState::onDeclineCallbackClicked()
    {
        context.setState<ConnectedState>();
    }

    void SendingSmsState::sendSms() {
        common::PhoneNumber receiverNumber = smsComposeMode.getPhoneNumber();
        std::string smsText = smsComposeMode.getSmsText();
        SMS sms = SMS(smsText,receiverNumber);
        context.user.getSmsDB().addSmsToDB(sms);
        context.logger.logInfo(smsText);
        context.logger.logInfo(receiverNumber);
        context.bts.sendSms(receiverNumber,smsText);
    }

}