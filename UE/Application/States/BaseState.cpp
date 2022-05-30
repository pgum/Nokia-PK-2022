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
    logger.logError("Uexpected: handleTimeout");
}

void BaseState::handleSib(common::BtsId btsId)
{
    logger.logError("Uexpected: handleSib: ", btsId);
}

void BaseState::handleAttachAccept()
{
    logger.logError("Uexpected: handleAttachAccept");
}

void BaseState::handleAttachReject()
{
    logger.logError("Uexpected: handleAttachReject");
}

void BaseState::handleDisconnected()
{
    logger.logInfo("Uexpected: handleDisconnected");
}

void BaseState::handleSMS(common::PhoneNumber from, std::string text, common::MessageId msgType)
{
    logger.logInfo("Uexpected: handleSMS");
}

void BaseState::handleCallRequest(common::PhoneNumber from)
{
    logger.logInfo("Uexpected: handleReceiveCallRequest");
}

void BaseState::handleCallAccepted(common::PhoneNumber)
{
    logger.logInfo("Uexpected: handleReceiveCallAccepted");
}

void BaseState::handleCallDropped(common::PhoneNumber)
{
    logger.logInfo("Uexpected: handleReceiveCallDropped");
}

void BaseState::handleUnknownRecipientAfterCallRequest()
{
    logger.logInfo("Uexpected: handleUnknownRecipientAfterCallRequest");
}

void BaseState::handleUnknownRecipientAfterCallAccepted()
{
    logger.logInfo("Uexpected: handleUnknownRecipientAfterCallAccepted");
}



}
