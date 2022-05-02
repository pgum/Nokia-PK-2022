#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "ViewingSmsState.hpp"

namespace ue
{

    ConnectedState::ConnectedState(Context &context)
        : BaseState(context, "ConnectedState")
    {
        context.user.acceptCallback([this] { onAcceptCallbackClicked(); });
        context.user.rejectCallback([this] { onDeclineCallbackClicked(); });
        context.user.showConnected();
    }

    void ConnectedState::onAcceptCallbackClicked()
    {
        int currentMenuIndex = context.user.getMenuIndex();

        if (currentMenuIndex == 1)
        {
            context.logger.logInfo("Konrad Was");
        }
        if (currentMenuIndex == 2)
        {
            context.setState<ViewingSmsState>();
        }
    }

    void ConnectedState::onDeclineCallbackClicked()
    {
        context.logger.logInfo("Jakub Pietrzko");
    }

    void ConnectedState::handleDisconnected()
    {
        context.setState<NotConnectedState>();
    }

}
