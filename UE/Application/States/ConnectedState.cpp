#include "ConnectedState.hpp"
#include "SendingSmsState.hpp"
#include "ViewSmsListState.hpp"
#include "IUeGui.hpp"
#include "NotConnectedState.hpp"
#include "BtsPort.hpp"
#include "TimerPort.hpp"

namespace {
    enum {
        SENDING_SMS = 0,
        VIEW_SMS_LIST = 1
    };
}

namespace ue {

    ConnectedState::ConnectedState(Context &context)
            : BaseState(context, "ConnectedState") {
        context.user.acceptCallback([this] { showSmsButton(); });
        context.user.rejectCallback([this] { closeSmsButton(); });
        context.user.showConnected();
    }

    void ConnectedState::handleDisconnected() {
        context.setState<NotConnectedState>();
    }

    common::PhoneNumber ConnectedState::getSenderPhoneNumber() {
        return this->senderPhoneNumber;
    }

    void ConnectedState::showSmsButton() {
        switch (context.user.getAction()) {
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

    void ConnectedState::handleSmsReceive(uint8_t action, const std::string &text, common::PhoneNumber fromPhoneNumber,
                                          common::PhoneNumber toPhoneNumber) {
        SmsDb &db = context.user.getSmsDb();
        db.addSms(text, fromPhoneNumber, toPhoneNumber);
    }

    void ConnectedState::handleFailedSmsSend() {
        SmsDb &db = context.user.getSmsDb();
        db.markLastSmsSentAsFailed();
    }

    void ConnectedState::handleCallAccept(common::PhoneNumber phoneNumber)
    {
        context.timer.stopTimer();
        std::cout<<"Accept call from:";
        context.bts.BTS_sendCallAccept(phoneNumber);
        context.user.callAchieved(phoneNumber);
        context.setState<TalkingState>(phoneNumber);
    }


    void ConnectedState::handleCallAccept(common::PhoneNumber phoneNumber)
    {
        context.timer.TIMER_stopTimer();
        context.user.callAchieved(phoneNumber);
        context.setState<TalkingState>(phoneNumber);
    }

    void ConnectedState::handleCallDrop(common::PhoneNumber phoneNumber)
    {
        context.timer.TIMER_stopTimer();
        std::printf("Accept call from:");
        context.user.showPartnerNotAvailable(phoneNumber);
    }

    void ConnectedState::handleUknownRecipient(common::PhoneNumber receiverPhoneNumber)
    {
        context.timer.TIMER_stopTimer();
        context.user.showPartnerNotAvailable(receiverPhoneNumber);
    }

    void ConnectedState::handleStartDial()
    {
        context.user.showEnterPhoneNumber();
    }

    void ConnectedState::handleCallRequest(common::PhoneNumber receiverPhoneNumber)
    {
        context.bts.sendCallRequest(receiverPhoneNumber);
        context.user.showDialing(receiverPhoneNumber);
    }

    void ConnectedState::handleCallDrop(common::PhoneNumber receiverPhoneNumber)
    {
        context.timer.stopTimer();
        context.bts.sendCallDrop(receiverPhoneNumber);
    }

}
