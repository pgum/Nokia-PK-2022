#include "ConnectedState.hpp"
#include "SendingCallRequest.hpp"

namespace ue {

    SendingCallRequestState::SendingCallRequestState(Context &context): BaseState(context, "SendingCallRequestState"),
    dialMode(context.user.dialComposeMode()) {
        context.user.acceptCallback([this] { onAcceptCallbackClicked(); });
        context.user.rejectCallback([this] { onDeclineCallbackClicked(); });
    }

    void SendingCallRequestState::onAcceptCallbackClicked() {
        makeCall();
    }

    void SendingCallRequestState::onDeclineCallbackClicked() {
        common::PhoneNumber receiverNumber = dialMode.getPhoneNumber();
        makeDropCall(receiverNumber);
    }

    void SendingCallRequestState::makeCall() {
        common::PhoneNumber receiverNumber = dialMode.getPhoneNumber();
        context.logger.logInfo("Calling to "+ common::to_string(receiverNumber));
        context.bts.makeCall(receiverNumber);
        context.user.callToMode(receiverNumber);
    }

    void SendingCallRequestState::handleDropCall(const common::PhoneNumber callerNumber) {
        context.setState<ConnectedState>(0);
    }

    void SendingCallRequestState::makeDropCall(const common::PhoneNumber callerNumber) {
        context.bts.declineCall(callerNumber);
        context.logger.logInfo("Dropping call from "+ common::to_string(callerNumber));
        context.setState<ConnectedState>(0);
    }
}