#include "TalkingState.hpp"
#include "ConnectedState.hpp"
#include <thread>

namespace ue {


    TalkingState::TalkingState(Context &context,common::PhoneNumber phoneNumber)
            : BaseState(context, "TalkingState"), caller{phoneNumber}
    {
        context.user.USER_startTalking(phoneNumber);
        std::cout<<"hello"<<std::endl;
    }

    void TalkingState::handleUknownRecipient(common::PhoneNumber recipientPhoneNumber)
    {
        context.timer.stopTimer();
        context.user.USER_showPartnerNotAvailable(recipientPhoneNumber);
        context.timer.startTimerAndDoSomething([this](){this->context.setState<ConnectedState>();},2);

    }

}
