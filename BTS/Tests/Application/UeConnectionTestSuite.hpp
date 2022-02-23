#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "UeConnection/UeConnection.hpp"

#include "Mocks/ITransportMock.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/UeSlotMock.hpp"
#include "Mocks/IUeConnectionMock.hpp"

namespace bts
{

using common::MessageId;

class UeConnectionTestSuite : public ::testing::Test
{
public:
    void assertDestruction();
    void SetUp() override;
    void TearDown() override;
protected:
    UeConnectionTestSuite();
    ~UeConnectionTestSuite();

    void expectRegisterCallbacks();
    void verifyAndClearExpectations();

    SyncGuardPtr syncGuard;
    const BtsId BTS_ID{17};
    const std::string TRANSPORT_ADDRESS = "CDEF";
    const PhoneNumber NO_PHONE{};
    const PhoneNumber PHONE{113};
    const PhoneNumber NOT_MY_PHONE{13};
    const PhoneNumber OTHER_PHONE{31};

    std::shared_ptr<IUeSlotImplMock> ueSlotNotAttachedMock;
    std::shared_ptr<IUeSlotImplMock> ueSlotFailedAttachedMock;
    std::shared_ptr<IUeSlotImplMock> ueSlotAttachedMock;
    std::shared_ptr<IUeSlotImplMock> ueSlotReattachedMock;

    std::shared_ptr<testing::StrictMock<common::ITransportMock>> transportMock;
    testing::NiceMock<common::ILoggerMock> loggerMock;

    ITransport::MessageCallback ueMessageCallback;
    ITransport::DisconnectedCallback ueDisconnectedCallback;

    std::unique_ptr<UeConnection> objectUnderTest;
};

class UeConnectionWithConnectedTransportTestSuite : public UeConnectionTestSuite
{
public:
    void SetUp() override;
protected:
    UeConnectionWithConnectedTransportTestSuite();

    const MessageId OTHER_THAN_ATTACH_REQUEST_MESSAGE = MessageId::CallTalk;

    void handleAttachRequest(PhoneNumber phoneNumber);
    void handleDisconnect();
    auto eqAttachResponseMessage(bool expectedAccepted);
    auto eqAttachResponseMessage(bool expectedAccepted, PhoneNumber expectedTo);
    BinaryMessage buildOtherThanAttachRequestMessage();
    BinaryMessage buildOtherThanAttachRequestMessage(PhoneNumber fromPhoneNumber);
};

class UeConnectionAttachedTestSuite : public UeConnectionWithConnectedTransportTestSuite
{
protected:
    UeConnectionAttachedTestSuite();
};

}
