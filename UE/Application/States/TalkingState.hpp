#pragma once

#include "BaseState.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue {
    class TalkingState : public BaseState {
    private:
        common::PhoneNumber caller;
    public:
        TalkingState(Context &context, common::PhoneNumber caller);

// IBtsEventsHandler interface
        void handleUknownRecipient(common::PhoneNumber) override;


    };

}