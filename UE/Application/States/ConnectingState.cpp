#include "ConnectingState.hpp"
#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"

namespace ue {

    ConnectingState::ConnectingState(Context &context, common::BtsId btsId)
            : BaseState(context, "ConnectingState") {
        context.user.USER_showConnecting();
        context.bts.sendAttachRequest(btsId);
        using namespace std::chrono_literals;
        context.timer.startTimer(0.5);
    }


    void ConnectingState::handleAttachAccept() {
        context.timer.stopTimer();
        context.setState<ConnectedState>();
    }

    void ConnectingState::handleAttachReject() {
        context.timer.stopTimer();
        context.setState<NotConnectedState>();
    }

    void ConnectingState::handleTimeout() {
        context.setState<NotConnectedState>();
    }
}



