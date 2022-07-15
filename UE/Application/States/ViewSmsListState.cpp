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

    void ViewSmsListState::handleSmsReceive(uint8_t action, const std::string& text, common::PhoneNumber fromPhoneNumber, common::PhoneNumber toPhoneNumber) {
        ConnectedState::handleSmsReceive(action, text, fromPhoneNumber, toPhoneNumber);
        context.setState<ViewSmsListState>();
    }
}