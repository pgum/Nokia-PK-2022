#include "NotConnectedState.hpp"

namespace ue
{

NotConnectedState::NotConnectedState(Context &context)
    : BaseState(context, "NotConnectedState")
{

}

void NotConnectedState::handleSib(common::BtsId btsId)
{
    context.bts.sendAttachRequest(btsId);
}

}
