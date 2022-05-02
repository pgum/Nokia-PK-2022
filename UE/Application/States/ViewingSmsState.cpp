#include "ConnectedState.hpp"
#include "ViewingSmsState.hpp"

namespace ue {

    ViewingSmsState::ViewingSmsState(Context &context)
            : BaseState(context, "ViewingSmsState") {
        context.user.acceptCallback([this] { onAcceptCallbackClicked(); });
        context.user.rejectCallback([this] { onDeclineCallbackClicked(); });
        context.user.showViewingSms();
    }

    void ViewingSmsState::onAcceptCallbackClicked()
    {
        int currentMenuIndex = context.user.getMenuIndex();

        if (currentMenuIndex == 1)
        {
            context.logger.logInfo("Konrad Was");
        }
        if (currentMenuIndex == 2)
        {
            context.logger.logInfo("Bartosz Preweda");
        }
    }

    void ViewingSmsState::onDeclineCallbackClicked()
    {
        context.setState<ConnectedState>();
    }
}