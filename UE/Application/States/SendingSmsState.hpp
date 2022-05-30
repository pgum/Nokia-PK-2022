#pragma once

#include "ConnectedState.hpp"
#include "UeGui/ISmsComposeMode.hpp"

namespace ue {
    class SendingSmsState : public ConnectedState {
    private:
        IUeGui::ISmsComposeMode& iSmsComposeMode;
    public:
        SendingSmsState(Context& context);

        void showSmsButton() override;
        void closeSmsButton() override;
    };
}