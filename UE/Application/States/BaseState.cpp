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
    void BaseState::handleCallRequest(common::PhoneNumber phoneNumber)
    {
        logger.logError("Uexpected: handleCallRequest: ",phoneNumber);
    }

    void BaseState::handleUknownRecipient(common::PhoneNumber recipientPhoneNumber)
    {
        logger.logError("Uexpected: handleUknownRecipient: ",recipientPhoneNumber);
    }

    void BaseState::handleCallAccept(common::PhoneNumber receiverPhoneNumber)
    {
        logger.logError("Uexpected: handleCallAccepted: ",receiverPhoneNumber);
    }

    void BaseState::handleCallAccept(common::PhoneNumber receiverPhoneNumber)
    {
        logger.logError("Uexpected: handleCallAccept: ",receiverPhoneNumber);
    }

    void BaseState::handleCallDrop(common::PhoneNumber receiverPhoneNumber)
    {
        logger.logError("Uexpected: handleCallDrop: ",receiverPhoneNumber);
    }

    void BaseState::handleStartDial()
    {
        logger.logError("Uexpected: handleStartDial");
    }

    void BaseState::handleCallRequest(common::PhoneNumber receiverPhoneNumber)
    {
        logger.logError("Uexpected: handleCallRequest: ",receiverPhoneNumber);
    }

    void BaseState::handleCallDrop(common::PhoneNumber receiverPhoneNumber)
    {
        logger.logError("Uexpected: handleCallDropSender: ",receiverPhoneNumber);
    }
}
