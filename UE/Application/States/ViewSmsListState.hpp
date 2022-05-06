#pragma once

#include "ConnectedState.hpp"

namespace ue {

    class ViewSmsListState : public ConnectedState {
    public:
        ViewSmsListState(Context& context);
        void showSmsButton() override;
        void closeSmsButton() override;
    };

}


