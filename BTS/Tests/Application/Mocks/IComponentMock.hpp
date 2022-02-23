#pragma once

#include <gmock/gmock.h>
#include "IComponent.hpp"

namespace bts
{

class IComponentMock : public IComponent
{
public:
    IComponentMock();
    ~IComponentMock() override;

    MOCK_METHOD(void, start, (), (final));
    MOCK_METHOD(void, stop, (), (final));
};


}
