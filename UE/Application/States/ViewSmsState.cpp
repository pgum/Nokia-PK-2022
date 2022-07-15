#include "ViewSmsState.hpp"
#include "ConnectedState.hpp"
#include "ViewSmsListState.hpp"

namespace ue {
    ViewSmsState::ViewSmsState(Context &context, int index) : ConnectedState(context) {
        context.user.showSms(index);
    }

    void ViewSmsState::showSmsButton() {

    }

    void ViewSmsState::closeSmsButton() {
        context.setState<ViewSmsListState>();
    }
}