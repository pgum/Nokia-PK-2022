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
        context.state->BTS_handleCallRequest(phoneNumber);
    }

    void Application::handleCallAccept(common::PhoneNumber phoneNumber) {
        context.state->USER_handleCallAccept(phoneNumber);
    }

    void Application::handleUknownRecipient(common::PhoneNumber recipientPhoneNumber) {
        context.state->BTS_handleUknownRecipient(recipientPhoneNumber);
    }

    void Application::handleCallAccept(common::PhoneNumber phoneNumber) {
        context.state->BTS_handleCallAccept(phoneNumber);
    }

    void Application::handleCallDrop(common::PhoneNumber phoneNumber) {
        context.state->BTS_handleCallDrop(phoneNumber);
    }

    void Application::USER_handleStartDial() {
        context.state->USER_handleStartDial();
    }

    void Application::USER_handleCallRequest(common::PhoneNumber receiverPhoneNumber) {
        context.state->USER_handleCallRequest(receiverPhoneNumber);
    }

    void Application::USER_handleCallDrop(common::PhoneNumber receiverPhoneNumber) {
        context.state->USER_handleCallDrop(receiverPhoneNumber);
    }

}




