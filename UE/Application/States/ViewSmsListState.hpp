#pragma once

#include "ConnectedState.hpp"

namespace ue {

    class ViewSmsListState : public ConnectedState {
    public:
        ViewSmsListState(Context& context);
        void showSmsButton() override;
        void closeSmsButton() override;
        void handleSmsReceive(uint8_t action, const std::string& text, common::PhoneNumber fromPhoneNumber, common::PhoneNumber toPhoneNumber) override;
    };

}


