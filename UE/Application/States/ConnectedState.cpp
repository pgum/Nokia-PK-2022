#include "ConnectedState.hpp"
#include "ViewSmsListState.hpp"
#include "IUeGui.hpp"
#include "NotConnectedState.hpp"

namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
{
    context.user.acceptCallback([this] { showSmsButton(); });
    context.user.rejectCallback([this] { closeSmsButton(); });
    context.user.showConnected();
}

void ConnectedState::showSmsButton() {
    if(context.user.getAction() == 1) {
        context.setState<ViewSmsListState>();
    }
}

void ConnectedState::closeSmsButton() {
    // TODO
}

void ConnectedState::handleSmsReceive(uint8_t action, std::string text) {
    SmsDb &db = context.user.getSmsDb();
    db.addSms(text);
}

void ConnectedState::handleDisconnected()
{
    context.setState<NotConnectedState>();
}

}
