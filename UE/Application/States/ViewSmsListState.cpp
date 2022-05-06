#include "ViewSmsListState.hpp"
#include "ConnectedState.hpp"
#include "ViewSmsState.hpp"

namespace ue {


    ViewSmsListState::ViewSmsListState(Context &context) : ConnectedState(context) {
        context.user.showSmsList();
    }

    void ViewSmsListState::showSmsButton() {
        int action = context.user.getAction();
        context.setState<ViewSmsState>(action);
    }

    void ViewSmsListState::closeSmsButton() {
        context.setState<ConnectedState>();
    }
}