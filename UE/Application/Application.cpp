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

    void Application::handleSMSReceive(const std::string smsText, const common::PhoneNumber senderNumber) {
        context.state->handleSMSReceive(smsText, senderNumber);
    }

    void Application::handleCallRequest(const common::PhoneNumber callerNumber) {
        context.state->handleCallRequest(callerNumber);
    }

    void Application::handleDropCall(const common::PhoneNumber callerNumber) {
        context.state->handleDropCall(callerNumber);
    }

    void Application::makeDropCall(const common::PhoneNumber callerNumber) {
        context.state->makeDropCall(callerNumber);
    }

}
