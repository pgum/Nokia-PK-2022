#include "TalkingState.hpp"
#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include <thread>

namespace ue {


    TalkingState::TalkingState(Context &context,common::PhoneNumber phoneNumber)
            : BaseState(context, "TalkingState"), caller{phoneNumber}
    {
        context.user.startTalking(phoneNumber);
        using namespace std::chrono_literals;
        context.timer.startTimer(120000ms);
    }

    void TalkingState::handleTimeout() {
        context.bts.sendCallDrop(caller);
        context.setState<ConnectedState>();
        context.user.showConnected();
    }

    void TalkingState::handleDisconnected() {
        context.setState<NotConnectedState>();
    }

    void TalkingState::handleUnknownRecipientCallTalk(common::PhoneNumber phoneNumber) {
        context.timer.stopTimer();
        context.user.showPartnerNotAvailable(phoneNumber);
        context.setState<ConnectedState>();
    }

    void TalkingState::handleSendCallTalk(common::PhoneNumber phoneNumber, std::string message) {
        context.timer.stopTimer();
        context.bts.sendCallTalk(phoneNumber, message);
        using namespace std::chrono_literals;
        context.timer.startTimer(120000ms);
    }

    void TalkingState::handleCallTalk(common::PhoneNumber phoneNumber, std::string message) {
        context.timer.stopTimer();
        context.user.showNewCallTalk(phoneNumber, message);
        using namespace std::chrono_literals;
        context.timer.startTimer(120000ms);
    }

    void TalkingState::handleCallDrop(common::PhoneNumber phoneNumber) {
        if(phoneNumber == caller){
            context.setState<ConnectedState>();
            context.user.showConnected();
        }
    }

    void TalkingState::handleSendCallDrop(common::PhoneNumber) {
        context.bts.sendCallDrop(caller);
        context.setState<ConnectedState>();
        context.user.showConnected();
    }

    void TalkingState::handleCallRequest(common::PhoneNumber phoneNumber) {
        context.bts.sendCallDrop(phoneNumber);
    }

    void TalkingState::handleSmsReceive(uint8_t action, const std::string &text, common::PhoneNumber fromPhoneNumber,
                                          common::PhoneNumber toPhoneNumber) {
        SmsDb &db = context.user.getSmsDb();
        db.addSms(text, fromPhoneNumber, toPhoneNumber);
    }

}
