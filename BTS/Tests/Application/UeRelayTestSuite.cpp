#include "UeRelayTestSuite.hpp"

using namespace ::testing;

namespace bts
{


UeRelayTestSuite::UeRelayTestSuite()
{
    objectUnderTest = std::make_unique<UeRelay>(loggerMock);
    verifyAndClearExpectations();

    connectionAdded.add(*objectUnderTest);

    connectionAttached.add(*objectUnderTest);
    connectionAttached.attach(ATTACHED_PHONE);

    connectionReAttached.add(*objectUnderTest);
    connectionReAttached.attach(ATTACHED_PHONE_2);
    connectionReAttached.attach(REATTACHED_PHONE);
}

void UeRelayTestSuite::verifyAndClearExpectations()
{
    Mock::VerifyAndClearExpectations(&loggerMock);
}

void UeRelayTestSuite::shallForwardMessage(ConnectionMock &connnection)
{
    connnection.expectSendMessage(MESSAGE);
    ASSERT_TRUE(connnection.connectionSlot.sendMessage(MESSAGE, connnection.phoneNumber));
}

void UeRelayTestSuite::shallNotForwardMessage(PhoneNumber phoneNumber)
{
    ASSERT_FALSE(connectionAttached.connectionSlot.sendMessage(MESSAGE, phoneNumber));
}

void UeRelayTestSuite::expectAction(UeRelayTestSuite::ConnectionMock &connnection)
{
    connnection.expectSendSib(BTS_ID);
}

IUeRelay::UeVisitor UeRelayTestSuite::getAction()
{
    return [this](IUeConnection& ue) { ue.sendSib(BTS_ID); };
}

UeRelayTestSuite::ConnectionMock::ConnectionMock()
    : connectionMock(new ::testing::StrictMock<IUeConnectionMock>()),
      connectionPtr(connectionMock)
{
    EXPECT_CALL(*connectionMock, print(_)).WillRepeatedly(Invoke(this, &UeRelayTestSuite::ConnectionMock::printConnection));
}

void UeRelayTestSuite::ConnectionMock::printConnection(std::ostream& os)
{
    os << phoneNumber;
}

void UeRelayTestSuite::ConnectionMock::add(IUeRelay &ueRelay)
{
    connectionSlot = ueRelay.add(std::move(connectionPtr));
}

void UeRelayTestSuite::ConnectionMock::attach(PhoneNumber phoneNumber)
{
    connectionSlot.attach(phoneNumber);
    this->phoneNumber = phoneNumber;
}

void UeRelayTestSuite::ConnectionMock::remove()
{
    connectionSlot.remove();
}

void UeRelayTestSuite::ConnectionMock::expectSendMessage(const BinaryMessage& message)
{
    auto matchMessage = Field(&BinaryMessage::value, (message.value));
    EXPECT_CALL(*connectionMock, sendMessage(matchMessage));
}

void UeRelayTestSuite::ConnectionMock::expectSendSib(BtsId btsId)
{
    EXPECT_CALL(*connectionMock, sendSib(btsId));
}

TEST_F(UeRelayTestSuite, shallNewlyAddedBeNotAttached)
{
    ASSERT_FALSE(connectionAdded.connectionSlot.isAttached());
}

TEST_F(UeRelayTestSuite, shallAttachedBeReportedAsAttached)
{
    ASSERT_TRUE(connectionAttached.connectionSlot.isAttached());
}

TEST_F(UeRelayTestSuite, shallReAttachedBeReportedAsAttached)
{
    ASSERT_TRUE(connectionReAttached.connectionSlot.isAttached());
}

TEST_F(UeRelayTestSuite, shallRemovedPhoneBeAvailableToUse)
{
    connectionAttached.connectionSlot.remove();

    ConnectionMock someNewConnection;
    someNewConnection.add(*objectUnderTest);
    someNewConnection.attach(ATTACHED_PHONE);

    ASSERT_TRUE(someNewConnection.connectionSlot.isAttached());
}

TEST_F(UeRelayTestSuite, shallAttachedPhoneBeUnavailableToUse)
{
    ConnectionMock someNewConnection;
    someNewConnection.add(*objectUnderTest);
    someNewConnection.attach(ATTACHED_PHONE);

    ASSERT_FALSE(someNewConnection.connectionSlot.isAttached());
}

TEST_F(UeRelayTestSuite, shallReAttachedToAttachedPhoneCauseDeattaching)
{
    ConnectionMock someNewConnection;
    someNewConnection.add(*objectUnderTest);
    someNewConnection.attach(ATTACHED_PHONE_2);
    someNewConnection.attach(ATTACHED_PHONE);

    ASSERT_FALSE(someNewConnection.connectionSlot.isAttached());
}

TEST_F(UeRelayTestSuite, shallNotForwardMessageToNotAttachedUe)
{
    shallNotForwardMessage(NOT_ATTACHED_PHONE);
}

TEST_F(UeRelayTestSuite, shallForwardMessageToAttachedUe)
{
    shallForwardMessage(connectionAttached);
}

TEST_F(UeRelayTestSuite, shallForwardMessageToReAttachedUe)
{
    shallForwardMessage(connectionReAttached);
}

TEST_F(UeRelayTestSuite, shallCountNotAttachedConnections)
{
    ASSERT_EQ(connectionAdded.count(), objectUnderTest->countNotAttached());
}

TEST_F(UeRelayTestSuite, shallCountAttachedConnections)
{
    ASSERT_EQ(connectionAttached.count() + connectionReAttached.count(),
              objectUnderTest->countAttached());
}

TEST_F(UeRelayTestSuite, shallCountConnections)
{
    ASSERT_EQ(connectionAdded.count() + connectionAttached.count() + connectionReAttached.count(),
              objectUnderTest->count());
}

TEST_F(UeRelayTestSuite, shallVisitNotAttachedConnections)
{
    expectAction(connectionAdded);
    objectUnderTest->visitNotAttachedUe(getAction());
}

TEST_F(UeRelayTestSuite, shallVisitAttachedConnections)
{
    expectAction(connectionAttached);
    expectAction(connectionReAttached);
    objectUnderTest->visitAttachedUe(getAction());
}

}
