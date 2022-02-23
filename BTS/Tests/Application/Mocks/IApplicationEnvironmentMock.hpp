#pragma once

#include <gmock/gmock.h>
#include "IApplicationEnvironment.hpp"

namespace bts
{

class IApplicationEnvironmentMock : public IApplicationEnvironment
{
public:
    IApplicationEnvironmentMock();
    ~IApplicationEnvironmentMock() override;

    MOCK_METHOD(IConsole&, getConsole, (), (final));
    MOCK_METHOD(void, registerUeConnectedCallback, (UeConnectedCallback), (final));
    MOCK_METHOD(ILogger&, getLogger, (), (final));
    MOCK_METHOD(BtsId, getBtsId, (), (const, final));
    MOCK_METHOD(std::string, getAddress, (), (const, final));
    MOCK_METHOD(void, startMessageLoop, (), (final));
};


}
