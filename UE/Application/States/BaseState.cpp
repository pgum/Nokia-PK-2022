#include "BaseState.hpp"

namespace ue {

    BaseState::BaseState(Context &context, const std::string &name)
            : context(context),
              logger(context.logger, "[" + name + "]") {
        logger.logDebug("entry");
    }

    BaseState::~BaseState() {
        logger.logDebug("exit");
    }

    void BaseState::handleTimeout() {
        logger.logError("Uexpected: handleTimeout");
    }

    void BaseState::handleDisconnected() {
        logger.logError("Uexpected: handleDisconnnected");
    }

    void BaseState::handleSib(common::BtsId btsId) {
        logger.logError("Uexpected: handleSib: ", btsId);
    }

    void BaseState::handleAttachAccept() {
        logger.logError("Uexpected: handleAttachAccept");
    }

    void BaseState::handleAttachReject() {
        logger.logError("Uexpected: handleAttachReject");
    }

    void BaseState::handleSMSReceive(const std::string smsText, const common::PhoneNumber senderNumber) {
        logger.logError("Uexpected: handleSMSReceive");
    }

    void BaseState::handleCallRequest(const common::PhoneNumber callerNumber) {
        logger.logError("Uexpected: handleCallRequest");
    }

    void BaseState::makeDropCall(const common::PhoneNumber callerNumber) {
        logger.logError("Uexpected: makeDropCall");
    }

    void BaseState::handleDropCall(const common::PhoneNumber callerNumber) {
        logger.logError("Uexpected: handleDropCall");
    }
}
