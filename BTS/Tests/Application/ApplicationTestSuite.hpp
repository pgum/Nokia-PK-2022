#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Application.hpp"

#include "Mocks/IComponentMock.hpp"
#include "Mocks/ILoggerMock.hpp"

namespace bts
{

class ApplicationTestSuite : public ::testing::Test
{
protected:
    ApplicationTestSuite();

    ::testing::NiceMock<common::ILoggerMock> loggerMock;
    std::shared_ptr<IComponentMock> componentMock1;
    std::shared_ptr<IComponentMock> componentMock2;
    std::shared_ptr<IComponentMock> componentMock3;

    std::unique_ptr<Application> objectUnderTest;
};


}
