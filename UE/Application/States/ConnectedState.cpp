#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "TalkingState.hpp"

namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
{
    context.user.showConnected();
}

void ConnectedState::handleDisconnected()
{
    context.setState<NotConnectedState>();
}

void ConnectedState::handleCallRequest(common::PhoneNumber from)
{
    using namespace std::chrono_literals;
    context.user.showNewCallRequest(from);
    context.timer.startTimer(30000ms);
}

void ConnectedState::handleSendCallAccept(common::PhoneNumber to)
{
    context.bts.sendCallAccept(to);
    context.timer.stopTimer();
    context.setState<TalkingState>();

    //TODO Wyświetlania okna rozmowy dla użytkownika
}

void ConnectedState::handleSendCallReject(common::PhoneNumber to)
{
    context.bts.sendCallReject(to);
    context.timer.stopTimer();

    //TODO Wyświetlania głownego menu
}

}
