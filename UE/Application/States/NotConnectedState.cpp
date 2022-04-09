#include "NotConnectedState.hpp"
#include "ConnectingState.hpp"


namespace ue
{

    NotConnectedState::NotConnectedState(Context &context)
        : BaseState(context, "NotConnectedState")
    {

    }

    void NotConnectedState::handleSib(common::BtsId btsId)
    {
        using namespace std::chrono_literals;
        context.timer.startTimer(500ms);
        context.bts.sendAttachRequest(btsId);

        context.setState<ConnectingState>();
    }

}
