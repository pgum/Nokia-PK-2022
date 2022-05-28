#include "ConnectedState.hpp"
#include "ViewingSmsState.hpp"
#include "ViewingSingleSmsState.hpp"

namespace ue {

    ViewingSingleSmsState::ViewingSingleSmsState(Context &context, int smsListPosition)
            : BaseState(context, "ViewingSingleSmsState") {
        context.user.acceptCallback([this] { onAcceptCallbackClicked(); });
        context.user.rejectCallback([this] { onDeclineCallbackClicked(); });
        context.user.showSingleSms(smsListPosition);
    }

    void ViewingSingleSmsState::onAcceptCallbackClicked() {}

    void ViewingSingleSmsState::onDeclineCallbackClicked() {
        context.setState<ViewingSmsState>();
    }

    void ViewingSingleSmsState::handleSMSReceive(const std::string smsText, const common::PhoneNumber senderNumber) {
        SMS sms1 = SMS(smsText, senderNumber, "received");
        SMS_DB ourDataBase = context.user.getSmsDB();
        ourDataBase.addSmsToDB(sms1);
        context.user.setSmsDB(ourDataBase);
    }
}