#pragma once

#include "BaseState.hpp"

namespace ue {

    class ViewingSmsState : public BaseState {
    public:
        ViewingSmsState(Context &context);

    protected:

        void onAcceptCallbackClicked();

        void onDeclineCallbackClicked();
    };
}