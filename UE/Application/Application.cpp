#include "Application.hpp"
#include "States/NotConnectedState.hpp"

namespace ue
{

Application::Application(common::PhoneNumber phoneNumber,
                         common::ILogger &iLogger,
                         IBtsPort &bts,
                         IUserPort &user,
                         ITimerPort &timer,
                         SmsDb& smsDB)
    : context{iLogger, bts, user, timer, smsDB},
      logger(iLogger, "[APP] ")
{
    logger.logInfo("Started");
    context.setState<NotConnectedState>();
}

Application::~Application()
{
    logger.logInfo("Stopped");
}

void Application::handleTimeout()
{
    context.state->handleTimeout();
    logger.logError("Timeout");
}

void Application::handleSib(common::BtsId btsId)
{
    logger.logInfo("Handling Sib");
    context.state->handleSib(btsId);
}

void Application::handleAttachAccept()
{
    context.state->handleAttachAccept();
    logger.logInfo("Attached");
}

void Application::handleAttachReject()
{
    context.state->handleAttachReject();
    logger.logError("Cannot attach");
}

void Application::handleDisconnected()
{
    logger.logInfo("Disconnecting");
    context.state->handleDisconnected();
}

}
