#pragma once

#include <gmock/gmock.h>
#include "UeRelay/IUeRelay.hpp"
#include "Printers/UeSlotPrint.hpp"

namespace bts
{

class IUeRelayMock : public IUeRelay
{
public:
    IUeRelayMock();
    ~IUeRelayMock() override;

    MOCK_METHOD(UeSlot, add, (UePtr), (final));

    MOCK_METHOD(std::size_t, count, (), (const, final));
    MOCK_METHOD(std::size_t, countAttached, (), (const, final));
    MOCK_METHOD(std::size_t, countNotAttached, (), (const, final));

    MOCK_METHOD(void, visitAttachedUe, (UeVisitor), (final));
    MOCK_METHOD(void, visitNotAttachedUe, (UeVisitor), (final));

    MOCK_METHOD(bool, sendMessage, (BinaryMessage message, PhoneNumber to), (final));


};


}
