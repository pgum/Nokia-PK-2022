#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <array>

#include "SibMolester.hpp"

#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IUeRelayMock.hpp"
#include "Mocks/IUeConnectionMock.hpp"

namespace bts
{

class SibMolesterTestSuite : public ::testing::Test
{
protected:
    SibMolesterTestSuite();

    static constexpr BtsId BTS_ID{17};
    static constexpr std::chrono::milliseconds TICK_DURATION{150};
    static constexpr std::chrono::milliseconds TICK_DURATION_MARGIN{20};
    static constexpr std::size_t TICKS_TO_SEND_SIB = 2;
    static constexpr std::size_t UE_NOT_ATTACHED_COUNT = 3;

    SyncGuardPtr syncGuard;
    std::shared_ptr<IUeRelayMock> ueRelayMock;
    testing::NiceMock<common::ILoggerMock> loggerMock;

    using UeNotAttached = std::array<testing::StrictMock<IUeConnectionMock>, UE_NOT_ATTACHED_COUNT>;
    UeNotAttached ueNotAttachedMock;

    std::unique_ptr<SibMolester> objectUnderTest;
    IUeRelay::UeVisitor visitor;
};

class SibMolesterStartedTestSuite : public SibMolesterTestSuite
{
protected:
    SibMolesterStartedTestSuite();
    void SetUp() override;
    void TearDown() override;
    void verifyAndClearExpectations();
    void expectVisitNotAttached();
    void expectUeSendSib(std::size_t ueIndex);
    void expectVisitAllNotAttachedUeAndSendSibForOne(std::size_t ueIndex);
};

}
