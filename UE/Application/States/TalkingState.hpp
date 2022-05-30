#pragma once

#include "BaseState.hpp"

namespace ue
{
class TalkingState : public BaseState
{
private:
    void handleUnknownRecipientAfterCallAccepted() override;
    void handleCallRequest(common::PhoneNumber from) override;
    void handleCallEnded ();

public:
        TalkingState(Context& context);

};

}
