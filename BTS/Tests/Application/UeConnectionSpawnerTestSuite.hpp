#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "UeConnection/UeConnectionSpawner.hpp"

#include "Mocks/ILoggerMock.hpp"
#include "Mocks/ITransportMock.hpp"
#include "Mocks/IUeConnectionMock.hpp"
#include "Mocks/IApplicationEnvironmentMock.hpp"
#include "Mocks/IUeRelayMock.hpp"
#include "Mocks/IUeConnectionFactoryMock.hpp"

namespace bts
{

class UeConnectionSpawnerTestSuite : public ::testing::Test
{
protected:
    UeConnectionSpawnerTestSuite();

    void setUpEnvironmentMock();
    void expectRegisterCallback();

    SyncGuardPtr syncGuard;
    const BtsId BTS_ID{17};

    ::testing::StrictMock<IApplicationEnvironmentMock> environmentMock;
    ::testing::NiceMock<common::ILoggerMock> loggerMock;
    std::shared_ptr<IUeRelayMock> ueRelayMock;
    std::shared_ptr<IUeConnectionFactoryMock> ueConnectionFactoryMock;
    UeConnectedCallback ueConnectedCallback;
    IUeRelay::UePtr ueConnection;

    std::unique_ptr<UeConnectionSpawner> objectUnderTest;
};

class UeConnectionStartedSpawnerTestSuite : public UeConnectionSpawnerTestSuite
{
protected:
    UeConnectionStartedSpawnerTestSuite();

    void onNewConnectionCallback();
    void expectUeCreated();
    void expectUeConnectedToTransport();
    void expectUeAddedToRelay();
    void expectSibSent();
    void assertUeDisconnectedOnDestroy();

    const UeSlot UE_SLOT;
    const std::string TRANSPORT_ADDRESS = "ABCDEF";

    std::shared_ptr<common::ITransportMock> transportMock;
    std::unique_ptr<IUeConnectionMock> ueConnectionMockPtr = std::make_unique<IUeConnectionMock>();
    IUeConnectionMock* ueConnectionMock = ueConnectionMockPtr.get();
    IUeRelay::UePtr ueConnection;

};


}
