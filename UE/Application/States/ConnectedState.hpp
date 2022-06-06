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

        void handleCallRequest(common::PhoneNumber) final;

        void handleCallAccept(common::PhoneNumber phoneNumber) final;

        void handleCallDrop(common::PhoneNumber phoneNumber) final;

        void handleUknownRecipient(common::PhoneNumber phoneNumber) final;


        // IUserEventsHandler interface
    public:
        void USER_handleCallAccept(common::PhoneNumber) final;

        void USER_handleStartDial() final;

        void USER_handleCallRequest(common::PhoneNumber) final;

        void USER_handleCallDrop(common::PhoneNumber) final;
    };

}
