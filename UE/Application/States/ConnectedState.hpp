#pragma once

#include "BaseState.hpp"

namespace ue {

    class ConnectedState : public BaseState {
    private :
        common: PhoneNumber senderPhoneNumber;

    public:
        ConnectedState(Context &context);

        void setSenderPhoneNumber(common::PhoneNumber senderPhoneNumber);

        common::PhoneNumber getSenderPhoneNumber();

        virtual void showSmsButton();

        virtual void closeSmsButton();

        void handleDisconnected() override;

        void handleSmsReceive(uint8_t action, const std::string &text, common::PhoneNumber fromPhoneNumber,
                              common::PhoneNumber toPhoneNumber) override;

        void handleFailedSmsSend();

        void handleCallRequest(common::PhoneNumber) override;

        void handleCallAccept(common::PhoneNumber phoneNumber) override;

        void handleCallDrop(common::PhoneNumber phoneNumber) override;

        void handleUknownRecipient(common::PhoneNumber phoneNumber) override;


        // IUserEventsHandler interface
    public:
        void handleCallAccept(common::PhoneNumber) override;

        void handleStartDial() override;

        void handleCallRequest(common::PhoneNumber) override;

        void handleCallDrop(common::PhoneNumber) override;
    };

}
