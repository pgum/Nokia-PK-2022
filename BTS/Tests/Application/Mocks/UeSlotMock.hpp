#pragma once

#include <gmock/gmock.h>
#include "UeConnection/UeSlot.hpp"
#include "Printers/UeSlotPrint.hpp"

namespace bts
{

class IUeSlotImplMock : public UeSlot::IImpl
{
public:
    IUeSlotImplMock();
    ~IUeSlotImplMock() override;

    MOCK_METHOD(bool, sendMessage, (BinaryMessage message, PhoneNumber to), (final));
    MOCK_METHOD(UeSlot::IImplPtr, attach, (PhoneNumber phone), (final));
    MOCK_METHOD(bool, isAttached, (), (const, final));
    MOCK_METHOD(PhoneNumber, getPhoneNumber, (), (const, final));
    MOCK_METHOD(void, remove, (), (final));
};


}
