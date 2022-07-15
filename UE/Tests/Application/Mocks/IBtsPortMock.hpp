#pragma once

#include <gmock/gmock.h>
#include "Ports/IBtsPort.hpp"
#include "Context.hpp"

namespace ue
{

class IBtsEventsHandlerMock : public IBtsEventsHandler
{
public:
    IBtsEventsHandlerMock();
    ~IBtsEventsHandlerMock() override;

    MOCK_METHOD(void, handleDisconnected, (), (final));
    MOCK_METHOD(void, handleSib, (common::BtsId), (final));
    MOCK_METHOD(void, handleAttachAccept, (), (final));
    MOCK_METHOD(void, handleAttachReject, (), (final));
    MOCK_METHOD(void, handleSmsReceive, (uint8_t, const std::string&, common::PhoneNumber, common::PhoneNumber), (final));
    MOCK_METHOD(void, handleFailedSmsSend, (), (final));
    MOCK_METHOD(void, handleCallRequest, (common::PhoneNumber), (final));
    MOCK_METHOD(void, handleUnknownRecipientCallRequest, (common::PhoneNumber), (final));
    MOCK_METHOD(void, handleUnknownRecipientCallTalk, (common::PhoneNumber), (final));
    MOCK_METHOD(void, handleCallAccept, (common::PhoneNumber), (final));
    MOCK_METHOD(void, handleCallDrop, (common::PhoneNumber), (final));
    MOCK_METHOD(void, handleCallTalk, (common::PhoneNumber, std::string), (final));
};

class IBtsPortMock : public IBtsPort
{
public:
    IBtsPortMock();
    ~IBtsPortMock() override;

    MOCK_METHOD(void, sendAttachRequest, (common::BtsId), (final));
    MOCK_METHOD(common::PhoneNumber, getOwnPhoneNumber, (), (final));
    MOCK_METHOD(void, sendSms, (common::PhoneNumber, std::string), (final));
    MOCK_METHOD(void, sendCallAccept, (common::PhoneNumber), (final));
    MOCK_METHOD(void, sendCallDrop, (common::PhoneNumber), (final));
    MOCK_METHOD(void, sendCallRequest, (common::PhoneNumber), (final));
    MOCK_METHOD(void, sendCallTalk, (common::PhoneNumber, std::string), (final));
};

}
