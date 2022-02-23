#pragma once

#include <gmock/gmock.h>
#include "CommonEnvironment/ITransport.hpp"

namespace common
{

struct ITransportMock : public ITransport
{
    ITransportMock();
    ~ITransportMock() override;

    MOCK_METHOD(void, registerMessageCallback, (MessageCallback), (final));
    MOCK_METHOD(void, registerDisconnectedCallback, (DisconnectedCallback), (final));
    MOCK_METHOD(bool, sendMessage, (BinaryMessage), (final));
    MOCK_METHOD(std::string, addressToString, (), (const, final));
};

}
