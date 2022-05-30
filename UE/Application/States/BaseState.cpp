#include "BaseState.hpp"

namespace ue
{

BaseState::BaseState(Context &context, const std::string &name)
    : context(context),
      logger(context.logger, "[" + name + "]")
{
    logger.logDebug("entry");
}

BaseState::~BaseState()
{
    logger.logDebug("exit");
}

void BaseState::handleTimeout()
{
    logger.logError("Unexpected: handleTimeout");
}

void BaseState::handleSib(common::BtsId btsId)
{
    logger.logError("Unexpected: handleSib: ", btsId);
}

void BaseState::handleAttachAccept()
{
    logger.logError("Unexpected: handleTimeout");
}

void BaseState::handleAttachReject()
{
    logger.logError("Unexpected: handleAttachReject");
}

void BaseState::handleDisconnected()
{
    logger.logError("Unexpected: handleDisconnected");
}

void BaseState::handleSmsReceive(uint8_t action, const std::string& text, common::PhoneNumber fromPhoneNumber, common::PhoneNumber toPhoneNumber) {
    logger.logError("Unexpected: handleSmsReceive");
}

void BaseState::handleFailedSmsSend()
{
    logger.logError("Unexpected: handleFailedSmsSend");
}

}
