#include "TalkingState.hpp"
#include "ConnectedState.hpp"

namespace ue
{
    TalkingState::TalkingState(Context &context)
    : BaseState(context, "TalkingState"){
    }

    void TalkingState::handleUnknownRecipientAfterCallAccepted()
    {
        context.timer.stopTimer();
        context.user.showPartnerNotAvailable();
        context.user.setAcceptCallback([&]{ handleCallEnded(); });
        context.user.setRejectCallback([&]{ handleCallEnded(); });
    }

    void TalkingState::handleCallRequest(common::PhoneNumber from)
    {
        context.bts.sendCallDropped(from);
    }

    void TalkingState::handleCallEnded()
    {
        context.currentCallingStatus.callingNumber.value = 0;
        context.currentCallingStatus.isOutgoingCall = false;
        context.setState<ConnectedState>();
    }
}
