#include "ApplicationTestSuite.hpp"

using namespace ::testing;

namespace bts
{

ApplicationTestSuite::ApplicationTestSuite()
{
    componentMock1 = std::make_shared<StrictMock<IComponentMock>>();
    componentMock2 = std::make_shared<StrictMock<IComponentMock>>();
    componentMock3 = std::make_shared<StrictMock<IComponentMock>>();
    std::initializer_list<std::shared_ptr<IComponent>> components = {componentMock1,
                                                                     componentMock2,
                                                                     componentMock3};

    objectUnderTest = std::make_unique<Application>(loggerMock, components);
}

TEST_F(ApplicationTestSuite, shallStartComponentsOnStart)
{
    InSequence seq;
    EXPECT_CALL(*componentMock1, start());
    EXPECT_CALL(*componentMock2, start());
    EXPECT_CALL(*componentMock3, start());

    objectUnderTest->start();
}

TEST_F(ApplicationTestSuite, shallStopComponentsOnStop)
{
    InSequence seq;
    EXPECT_CALL(*componentMock3, stop());
    EXPECT_CALL(*componentMock2, stop());
    EXPECT_CALL(*componentMock1, stop());

    objectUnderTest->stop();
}

}
