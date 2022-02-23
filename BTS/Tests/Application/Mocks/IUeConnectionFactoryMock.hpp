#pragma once

#include <gmock/gmock.h>
#include "UeConnection/IUeConnectionFactory.hpp"

namespace bts
{

class IUeConnectionFactoryMock : public IUeConnectionFactory
{
public:
    IUeConnectionFactoryMock();
    ~IUeConnectionFactoryMock() override;

    MOCK_METHOD(IUeRelay::UePtr, createConnection, (ITransportPtr), (final));
};


}
