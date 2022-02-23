#include "UeConnectionSpawnerTestSuite.hpp"
#include "Messages/MessageHeader.hpp"

using namespace ::testing;

namespace bts
{

UeConnectionSpawnerTestSuite::UeConnectionSpawnerTestSuite()
{
    setUpEnvironmentMock();
    syncGuard = std::make_shared<SyncGuard>();
    ueRelayMock = std::make_shared<StrictMock<IUeRelayMock>>();
    ueConnectionFactoryMock = std::make_shared<StrictMock<IUeConnectionFactoryMock>>();
    objectUnderTest = std::make_unique<UeConnectionSpawner>(environmentMock, ueConnectionFactoryMock, ueRelayMock, syncGuard);
}

void UeConnectionSpawnerTestSuite::setUpEnvironmentMock()
{
    EXPECT_CALL(environmentMock, getLogger()).WillRepeatedly(ReturnRef(loggerMock));
    EXPECT_CALL(environmentMock, getBtsId()).WillRepeatedly(Return(BTS_ID));
}

void UeConnectionSpawnerTestSuite::expectRegisterCallback()
{
    EXPECT_CALL(environmentMock, registerUeConnectedCallback(_)).WillOnce(SaveArg<0>(&ueConnectedCallback));
}

TEST_F(UeConnectionSpawnerTestSuite, shallRegisterOnStart)
{
    expectRegisterCallback();
    objectUnderTest->start();
    ASSERT_NE(nullptr, ueConnectedCallback);
}

TEST_F(UeConnectionSpawnerTestSuite, shallDeregisterOnStop)
{
    expectRegisterCallback();
    objectUnderTest->stop();
    ASSERT_FALSE(ueConnectedCallback);
}


UeConnectionStartedSpawnerTestSuite::UeConnectionStartedSpawnerTestSuite()
{
    expectRegisterCallback();
    objectUnderTest->start();

    transportMock = std::make_shared<testing::StrictMock<common::ITransportMock>>();
    EXPECT_CALL(*transportMock, addressToString()).WillRepeatedly(Return(TRANSPORT_ADDRESS));
}

void UeConnectionStartedSpawnerTestSuite::expectUeCreated()
{
    auto expectedTransport = Property(&ITransportPtr::get, transportMock.get());
    EXPECT_CALL(*ueConnectionFactoryMock, createConnection(expectedTransport)).WillOnce([this](auto&&) { return std::move(ueConnectionMockPtr); });
}

void UeConnectionStartedSpawnerTestSuite::expectUeConnectedToTransport()
{
    auto matchUeSlot = Property(&UeSlot::isAttached, UE_SLOT.isAttached());
    EXPECT_CALL(*ueConnectionMock, start(matchUeSlot));
}

void UeConnectionStartedSpawnerTestSuite::expectUeAddedToRelay()
{
    auto expectedConnection = Property(&IUeRelay::UePtr::get, ueConnectionMock);
    EXPECT_CALL(*ueRelayMock, add(expectedConnection)).WillOnce([this](auto p_arg)
    {
        this->ueConnection = std::move(p_arg);
        return UE_SLOT;
    });
}

void UeConnectionStartedSpawnerTestSuite::expectSibSent()
{
    EXPECT_CALL(*ueConnectionMock, sendSib(BTS_ID));
}

void UeConnectionStartedSpawnerTestSuite::onNewConnectionCallback()
{
    ueConnectedCallback(transportMock);
}

TEST_F(UeConnectionStartedSpawnerTestSuite, shallSpawnConnectionOnNewTransport)
{
    expectUeCreated();
    expectUeAddedToRelay();
    expectUeConnectedToTransport();
    expectSibSent();

    onNewConnectionCallback();
}

}
