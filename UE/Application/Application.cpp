#include "Application.hpp"
#include "States/NotConnectedState.hpp"

namespace ue {

    Application::Application(common::PhoneNumber phoneNumber,
                             common::ILogger &iLogger,
                             IBtsPort &bts,
                             IUserPort &user,
                             ITimerPort &timer)
            : context{iLogger, bts, user, timer},
              logger(iLogger, "[APP] ") {
        logger.logInfo("Started");
        context.setState<NotConnectedState>();
    }

    Application::~Application() {
        logger.logInfo("Stopped");
    }

    void Application::handleTimeout() {
        context.state->handleTimeout();
    }

    void Application::handleSib(common::BtsId btsId) {
        context.state->handleSib(btsId);
    }

    void Application::handleAttachAccept() {
        context.state->handleAttachAccept();
    }

    void Application::handleAttachReject() {
        context.state->handleAttachReject();
    }

    void Application::handleDisconnected() {
        context.state->handleDisconnected();
    }

    void Application::handleSmsReceive(uint8_t action, const std::string &text, common::PhoneNumber fromPhoneNumber,
                                       common::PhoneNumber toPhoneNumber) {
        context.state->handleSmsReceive(action, text, fromPhoneNumber, toPhoneNumber);
    }

    void Application::handleFailedSmsSend() {
        context.state->handleFailedSmsSend();
    }

    void Application::handleCallRequest(common::PhoneNumber phoneNumber) {
        context.state->handleCallRequest(phoneNumber);
    }

    void Application::handleCallAccept(common::PhoneNumber phoneNumber) {
        context.state->handleCallAccept(phoneNumber);
    }

    void Application::handleCallTalk(common::PhoneNumber phoneNumber, std::string message){
        context.state->handleCallTalk(phoneNumber, message);
    }


    void Application::handleCallDrop(common::PhoneNumber phoneNumber) {
        context.state->handleCallDrop(phoneNumber);
    }

    void Application::handleSendCallAccept(common::PhoneNumber phoneNumber) {
        context.state->handleSendCallAccept(phoneNumber);
    }

    void Application::handleStartDial() {
        context.state->handleStartDial();
    }

    void Application::handleSendCallRequest(common::PhoneNumber receiverPhoneNumber) {
        context.state->handleSendCallRequest(receiverPhoneNumber);
    }

    void Application::handleSendCallDrop(common::PhoneNumber receiverPhoneNumber) {
        context.state->handleSendCallDrop(receiverPhoneNumber);
    }

    void Application::handleSendCallTalk(common::PhoneNumber receiverPhoneNumber, std::string message) {
        context.state->handleSendCallTalk(receiverPhoneNumber, message);
    }

    void Application::handleUnknownRecipientCallRequest(common::PhoneNumber phoneNumber) {
        context.state->handleUnknownRecipientCallRequest(phoneNumber);
    }

    void Application::handleUnknownRecipientCallTalk(common::PhoneNumber phoneNumber){
        context.state->handleUnknownRecipientCallTalk(phoneNumber);
    }

}




