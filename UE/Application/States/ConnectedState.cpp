#include "ConnectedState.hpp"
#include "SendingSmsState.hpp"
#include "ViewSmsListState.hpp"
#include "IUeGui.hpp"
#include "NotConnectedState.hpp"
#include "TalkingState.hpp"

namespace {
    enum {
        SENDING_SMS = 0,
        VIEW_SMS_LIST = 1,
        DIAL = 2
    };
}

namespace ue {

    ConnectedState::ConnectedState(Context &context)
            : BaseState(context, "ConnectedState") {
        context.user.acceptCallback([this] { showSmsButton(); });
        context.user.rejectCallback([this] { closeSmsButton(); });
        context.user.showConnected();
        setSenderPhoneNumber({});
    }

    void ConnectedState::handleDisconnected() {
        context.setState<NotConnectedState>();
    }


    void ConnectedState::handleTimeout(){
        context.user.showConnected();
        handleSendCallDrop({});
    }

    void ConnectedState::setSenderPhoneNumber(common::PhoneNumber phoneNumber){
        this->senderPhoneNumber = phoneNumber;
    }

    void ConnectedState::showSmsButton() {
        switch (context.user.getAction()) {
            case SENDING_SMS:
                context.setState<SendingSmsState>();
                break;
            case VIEW_SMS_LIST:
                context.setState<ViewSmsListState>();
                break;
            case DIAL:
                context.user.showEnterPhoneNumber();
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

    void ConnectedState::handleCallRequest(common::PhoneNumber phoneNumber) {
        if(senderPhoneNumber.value != 0) context.bts.sendCallDrop(phoneNumber);
        else{
            using namespace std::chrono_literals;
            context.timer.startTimer(30000ms);
            context.user.showCallRequest(phoneNumber);
            setSenderPhoneNumber(phoneNumber);
        }
    }

    void ConnectedState::handleCallDrop(common::PhoneNumber phoneNumber) {

        context.user.showPartnerNotAvailable(phoneNumber);
        setSenderPhoneNumber({});
        context.user.showConnected();
        context.timer.stopTimer();
    }

    void ConnectedState::handleUnknownRecipientCallRequest(common::PhoneNumber phoneNumber) {
        context.timer.stopTimer();
        context.user.showPartnerNotAvailable(senderPhoneNumber);
        setSenderPhoneNumber({});
    }

    void ConnectedState::handleSendCallAccept(common::PhoneNumber phoneNumber)
    {
        context.timer.stopTimer();
        setSenderPhoneNumber({});
        context.user.callAchieved(phoneNumber);
        context.bts.sendCallAccept(phoneNumber);
        context.setState<TalkingState>(phoneNumber);
    }


    void ConnectedState::handleCallAccept(common::PhoneNumber phoneNumber)
    {
        context.timer.stopTimer();
        setSenderPhoneNumber({});
        std::cout<<"Accept call from:";
        context.user.callAchieved(phoneNumber);
        context.setState<TalkingState>(phoneNumber);
    }

    void ConnectedState::handleStartDial()
    {
        context.user.showEnterPhoneNumber();
    }

    void ConnectedState::handleSendCallRequest(common::PhoneNumber receiverPhoneNumber)
    {
        using namespace std::chrono_literals;
        context.timer.startTimer(60000ms);
        context.bts.sendCallRequest(receiverPhoneNumber);
        setSenderPhoneNumber(receiverPhoneNumber);
        context.user.showDialing(receiverPhoneNumber);
    }

    void ConnectedState::handleSendCallDrop(common::PhoneNumber receiverPhoneNumber)
    {
        context.timer.stopTimer();
        context.bts.sendCallDrop(senderPhoneNumber);
        setSenderPhoneNumber({});
        context.user.showConnected();
    }

}
