#include "ConnectedState.hpp"
#include "SendingSmsState.hpp"
#include "ViewSmsListState.hpp"
#include "IUeGui.hpp"
#include "NotConnectedState.hpp"

namespace {
    enum {
        SENDING_SMS = 0,
        VIEW_SMS_LIST = 1
    };
}

namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
{
    context.user.acceptCallback([this] { showSmsButton(); });
    context.user.rejectCallback([this] { closeSmsButton(); });
    context.user.showConnected();
}

void ConnectedState::handleDisconnected()
{
    context.setState<NotConnectedState>();
}

void ConnectedState::showSmsButton() {
    switch(context.user.getAction()){
        case SENDING_SMS:
            context.setState<SendingSmsState>();
            break;
        case VIEW_SMS_LIST:
            context.setState<ViewSmsListState>();
            break;
    }
}

void ConnectedState::closeSmsButton() {
    // TODO handle for clicking close sms button
}

void ConnectedState::handleSmsReceive(uint8_t action, const std::string& text, common::PhoneNumber fromPhoneNumber, common::PhoneNumber toPhoneNumber) {
    SmsDb &db = context.user.getSmsDb();
    db.addSms(text, fromPhoneNumber, toPhoneNumber);
}

void ConnectedState::handleFailedSmsSend()
{
    SmsDb &db = context.user.getSmsDb();
    db.markLastSmsSentAsFailed();
}

}
