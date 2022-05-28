#include "ConnectedState.hpp"
#include "ReceivingCallRequest.hpp"

namespace ue {

    ReceivingCallRequest::ReceivingCallRequest(Context &context,const common::PhoneNumber callerNumber)
            : BaseState(context, "ReceivingCallRequest"), alertMode(context.user.callFromMode(callerNumber)),
              callerNumber(callerNumber) {
        context.user.acceptCallback([this] { onAcceptCallbackClicked(); });
        context.user.rejectCallback([this] { onDeclineCallbackClicked(); });
    }

    void ReceivingCallRequest::onAcceptCallbackClicked(){}

    void ReceivingCallRequest::onDeclineCallbackClicked()
    {
        makeDropCall(callerNumber);
    }

    void ReceivingCallRequest::handleCallRequest(const common::PhoneNumber callerNumber) {
        context.logger.logInfo("Handling call request from "+ common::to_string(callerNumber));
    }

    void ReceivingCallRequest::makeDropCall(const common::PhoneNumber callerNumber) {
        context.bts.declineCall(callerNumber);
        context.logger.logInfo("Dropping call from " + common::to_string(callerNumber));
        context.setState<ConnectedState>(0);
    }

    void ReceivingCallRequest::handleDropCall(const common::PhoneNumber callerNumber) {
        context.setState<ConnectedState>(0);
    }
}