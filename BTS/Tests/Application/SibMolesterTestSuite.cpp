#include "SibMolesterTestSuite.hpp"

using namespace ::testing;

namespace bts
{

constexpr BtsId SibMolesterTestSuite::BTS_ID;
constexpr std::chrono::milliseconds SibMolesterTestSuite::TICK_DURATION;
constexpr std::chrono::milliseconds SibMolesterTestSuite::TICK_DURATION_MARGIN;
constexpr std::size_t SibMolesterTestSuite::TICKS_TO_SEND_SIB;
constexpr std::size_t SibMolesterTestSuite::UE_NOT_ATTACHED_COUNT;

SibMolesterTestSuite::SibMolesterTestSuite()
{
    syncGuard = std::make_shared<SyncGuard>();
    ueRelayMock = std::make_shared<StrictMock<IUeRelayMock>>();
    objectUnderTest = std::make_unique<SibMolester>(ueRelayMock, syncGuard, BTS_ID, loggerMock,
                                                    TICK_DURATION, TICKS_TO_SEND_SIB);
}

TEST_F(SibMolesterTestSuite, shallDoNothingWhenNotStarted)
{
    std::this_thread::sleep_for(TICKS_TO_SEND_SIB * TICK_DURATION + TICK_DURATION_MARGIN);
}

SibMolesterStartedTestSuite::SibMolesterStartedTestSuite()
{
}

void SibMolesterStartedTestSuite::SetUp()
{
    SibMolesterTestSuite::SetUp();

    EXPECT_CALL(*ueRelayMock, countNotAttached()).WillRepeatedly(Return(UE_NOT_ATTACHED_COUNT));
    for (auto& ue : ueNotAttachedMock)
        EXPECT_CALL(ue, print(_)).Times(AnyNumber());

    objectUnderTest->start();
}

void SibMolesterStartedTestSuite::TearDown()
{
    objectUnderTest->stop();
    SibMolesterTestSuite::TearDown();
}

void SibMolesterStartedTestSuite::verifyAndClearExpectations()
{
    Mock::VerifyAndClearExpectations(&ueRelayMock);
    for (auto& ue : ueNotAttachedMock)
        Mock::VerifyAndClearExpectations(&ue);
}

void SibMolesterStartedTestSuite::expectUeSendSib(std::size_t ueIndex)
{
    auto& expectedUe = ueNotAttachedMock[ueIndex];

    EXPECT_CALL(expectedUe, sendSib(BTS_ID));
    for (auto& ue : ueNotAttachedMock)
        visitor(ue);
    Mock::VerifyAndClearExpectations(&expectedUe);
    EXPECT_CALL(expectedUe, print(_)).Times(AnyNumber());
}

void SibMolesterStartedTestSuite::expectVisitNotAttached()
{
    visitor = nullptr;
    EXPECT_CALL(*ueRelayMock, visitNotAttachedUe(_)).WillOnce(SaveArg<0>(&visitor));
}

void SibMolesterStartedTestSuite::expectVisitAllNotAttachedUeAndSendSibForOne(std::size_t ueIndex)
{
    expectVisitNotAttached();
    std::this_thread::sleep_for(TICKS_TO_SEND_SIB * TICK_DURATION + TICK_DURATION_MARGIN);
    Mock::VerifyAndClearExpectations(&ueRelayMock);
    ASSERT_NE(nullptr, visitor);
    expectUeSendSib(ueIndex);
}

TEST_F(SibMolesterStartedTestSuite, shallNotSendSibAfterFirstTick)
{
    std::this_thread::sleep_for(TICK_DURATION + TICK_DURATION_MARGIN);
}

TEST_F(SibMolesterStartedTestSuite, shallSendSibAfterFirstFullDuration)
{
    expectVisitAllNotAttachedUeAndSendSibForOne(0);
}

TEST_F(SibMolesterStartedTestSuite, shallSendSibForTwoUeAfterTwoFullDurations)
{
    expectVisitAllNotAttachedUeAndSendSibForOne(0);
    expectVisitAllNotAttachedUeAndSendSibForOne(1);
}

TEST_F(SibMolesterStartedTestSuite, shallSendSibsInRound)
{
    static_assert((2 + UE_NOT_ATTACHED_COUNT) * TICK_DURATION_MARGIN < TICK_DURATION,
                  "Would because margin is too wide");

    for (std::size_t i = 0; i < UE_NOT_ATTACHED_COUNT; ++i)
        expectVisitAllNotAttachedUeAndSendSibForOne(i);
    expectVisitAllNotAttachedUeAndSendSibForOne(0);
    expectVisitAllNotAttachedUeAndSendSibForOne(1);
}

}
