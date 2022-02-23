#pragma once

#include <gmock/gmock.h>
#include "UeConnection/IUeConnection.hpp"
#include "Printers/UeSlotPrint.hpp"

namespace bts
{

class IUeConnectionMock : public IUeConnection
{
public:
    IUeConnectionMock();
    ~IUeConnectionMock() override;

    MOCK_METHOD(void, start, (UeSlot ueSlot), (final));
    MOCK_METHOD(void, sendMessage, (BinaryMessage message), (final));
    MOCK_METHOD(void, sendSib, (BtsId btsId), (final));
    MOCK_METHOD(PhoneNumber, getPhoneNumber, (), (const, final));
    MOCK_METHOD(bool, isAttached, (), (const, final));
    MOCK_METHOD(void, print, (std::ostream&), (const, final));
};


}
