#include "ConnectedState.hpp"
#include "ViewingSmsState.hpp"
#include "ViewingSingleSmsState.hpp"

namespace ue {

    ViewingSmsState::ViewingSmsState(Context &context)
            : BaseState(context, "ViewingSmsState") {
        context.user.acceptCallback([this] { onAcceptCallbackClicked(); });
        context.user.rejectCallback([this] { onDeclineCallbackClicked(); });
        context.user.showViewingSms();
    }

    void ViewingSmsState::onAcceptCallbackClicked() {
        int currentMenuIndex = context.user.getMenuIndex();
        context.setState<ViewingSingleSmsState>(currentMenuIndex + 1);
    }

    void ViewingSmsState::onDeclineCallbackClicked() {
        context.setState<ConnectedState>(0);
    }

    void ViewingSmsState::handleSMSReceive(const std::string smsText, const common::PhoneNumber senderNumber) {
        SMS sms = SMS(smsText, senderNumber, "received");
        SMS_DB ourDataBase = context.user.getSmsDB();
        ourDataBase.addSmsToDB(sms);
        context.user.setSmsDB(ourDataBase);
        context.user.showViewingSms();
    }
}