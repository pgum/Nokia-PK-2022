#include "UeConnectionTestSuite.hpp"
#include "Messages/IncomingMessage.hpp"
#include "Messages/OutgoingMessage.hpp"
#include "Mocks/UeSlotMock.hpp"

using namespace ::testing;

namespace bts
{
using common::IncomingMessage;
using common::OutgoingMessage;

namespace
{

constexpr std::size_t HEADER_SIZE = sizeof(MessageHeader::messageId)
                                  + sizeof(MessageHeader::from)
                                  + sizeof(MessageHeader::to);

MATCHER_P4(EqMessageHeader, offset, expectedMessage, expectedFrom, expectedTo, "")
{
    IncomingMessage actualMessage(arg);
    Matcher<MessageHeader> matcher = AllOf(
                Field(&MessageHeader::messageId, expectedMessage),
                Field(&MessageHeader::from, expectedFrom),
                Field(&MessageHeader::to, expectedTo));
    actualMessage.readText(offset);
    return matcher.MatchAndExplain(actualMessage.readMessageHeader(), result_listener);
}

MATCHER_P2(EqMessageNumber, offset, expectedNumber, "")
{
    using Number = std::remove_const_t<decltype(expectedNumber)>;
    IncomingMessage actualMessage(arg);
    Matcher<Number> matcher = expectedNumber;
    actualMessage.readText(offset);
    return matcher.MatchAndExplain(actualMessage.readNumber<Number>(), result_listener);
}

MATCHER_P2(EqMessageBtsId, offset, expectedBtsId, "")
{
    IncomingMessage actualMessage(arg);
    Matcher<BtsId> matcher = expectedBtsId;
    actualMessage.readText(offset);
    return matcher.MatchAndExplain(actualMessage.readBtsId(), result_listener);
}

}

UeConnectionTestSuite::UeConnectionTestSuite()
{
    ueSlotNotAttachedMock = std::make_shared<StrictMock<IUeSlotImplMock>>();
    ueSlotFailedAttachedMock = std::make_shared<StrictMock<IUeSlotImplMock>>();
    ueSlotAttachedMock = std::make_shared<StrictMock<IUeSlotImplMock>>();
    ueSlotReattachedMock = std::make_shared<StrictMock<IUeSlotImplMock>>();
    syncGuard = std::make_shared<SyncGuard>();
    transportMock = std::make_shared<StrictMock<common::ITransportMock>>();
    objectUnderTest = std::make_unique<UeConnection>(transportMock, loggerMock, syncGuard);
    verifyAndClearExpectations();
}

UeConnectionTestSuite::~UeConnectionTestSuite()
{}

void UeConnectionTestSuite::SetUp()
{
    EXPECT_CALL(*transportMock, addressToString()).WillRepeatedly(Return(TRANSPORT_ADDRESS));
    EXPECT_CALL(*ueSlotNotAttachedMock, isAttached()).WillRepeatedly(Return(false));
    EXPECT_CALL(*ueSlotFailedAttachedMock, isAttached()).WillRepeatedly(Return(false));
    EXPECT_CALL(*ueSlotAttachedMock, isAttached()).WillRepeatedly(Return(true));
    EXPECT_CALL(*ueSlotReattachedMock, isAttached()).WillRepeatedly(Return(true));

    EXPECT_CALL(*ueSlotNotAttachedMock, getPhoneNumber()).WillRepeatedly(Return(PhoneNumber{}));
    EXPECT_CALL(*ueSlotFailedAttachedMock, getPhoneNumber()).WillRepeatedly(Return(PhoneNumber{}));
    EXPECT_CALL(*ueSlotAttachedMock, getPhoneNumber()).WillRepeatedly(Return(PHONE));
    EXPECT_CALL(*ueSlotReattachedMock, getPhoneNumber()).WillRepeatedly(Return(OTHER_PHONE));
}

void UeConnectionTestSuite::assertDestruction()
{
    expectRegisterCallbacks();
    objectUnderTest.reset();
    ASSERT_FALSE(ueDisconnectedCallback);
    ASSERT_FALSE(ueMessageCallback);
    verifyAndClearExpectations();
}

void UeConnectionTestSuite::TearDown()
{
    assertDestruction();
    Test::TearDown();
}

void UeConnectionTestSuite::expectRegisterCallbacks()
{
    EXPECT_CALL(*transportMock, registerDisconnectedCallback(_)).WillOnce(SaveArg<0>(&ueDisconnectedCallback));
    EXPECT_CALL(*transportMock, registerMessageCallback(_)).WillOnce(SaveArg<0>(&ueMessageCallback));
}

void UeConnectionTestSuite::verifyAndClearExpectations()
{
    Mock::VerifyAndClearExpectations(transportMock.get());
    Mock::VerifyAndClearExpectations(&loggerMock);
}

TEST_F(UeConnectionTestSuite, shallSendMessage)
{
    BinaryMessage EXPECTED_MESSAGE = { {1,2,3,4,5,6} };
    auto matchMessage = Field(&BinaryMessage::value,
                              ContainerEq(EXPECTED_MESSAGE.value));

    EXPECT_CALL(*transportMock, sendMessage(matchMessage));

    objectUnderTest->sendMessage(EXPECTED_MESSAGE);
}

TEST_F(UeConnectionTestSuite, shallSendSibWithBtsId)
{
    auto EqSib = AllOf(EqMessageHeader(0, MessageId::Sib, NO_PHONE, NO_PHONE),
                       EqMessageBtsId(HEADER_SIZE, BTS_ID));
    EXPECT_CALL(*transportMock, sendMessage(EqSib));
    objectUnderTest->sendSib(BTS_ID);
}


TEST_F(UeConnectionTestSuite, shallConnectToTransportOnStart)
{
    expectRegisterCallbacks();
    objectUnderTest->start(UeSlot(ueSlotNotAttachedMock));
}

UeConnectionWithConnectedTransportTestSuite::UeConnectionWithConnectedTransportTestSuite()
{
    expectRegisterCallbacks();
    objectUnderTest->start(UeSlot(ueSlotNotAttachedMock));
    verifyAndClearExpectations();
}

void UeConnectionWithConnectedTransportTestSuite::SetUp()
{
    UeConnectionTestSuite::SetUp();
}

auto UeConnectionWithConnectedTransportTestSuite::eqAttachResponseMessage(bool expectedAccepted,
                                                                          PhoneNumber expectedTo)
{
    return AllOf(EqMessageHeader(0, MessageId::AttachResponse, NO_PHONE, expectedTo),
                 EqMessageNumber(HEADER_SIZE, expectedAccepted));
}

auto UeConnectionWithConnectedTransportTestSuite::eqAttachResponseMessage(bool expectedAccepted)
{
    return eqAttachResponseMessage(expectedAccepted, PHONE);
}

void UeConnectionWithConnectedTransportTestSuite::handleAttachRequest(PhoneNumber phoneNumber)
{
    OutgoingMessage attachRequestBuilder(MessageId::AttachRequest, phoneNumber, PhoneNumber{});
    auto attachRequestMessage = attachRequestBuilder.getMessage();
    ueMessageCallback(attachRequestMessage);
}

void UeConnectionWithConnectedTransportTestSuite::handleDisconnect()
{
    ueDisconnectedCallback();
}

BinaryMessage UeConnectionWithConnectedTransportTestSuite::buildOtherThanAttachRequestMessage(PhoneNumber from)
{
    OutgoingMessage messageBuilder(OTHER_THAN_ATTACH_REQUEST_MESSAGE, from, OTHER_PHONE);
    messageBuilder.writeText("ABCDE");

    return messageBuilder.getMessage();
}

BinaryMessage UeConnectionWithConnectedTransportTestSuite::buildOtherThanAttachRequestMessage()
{
    return buildOtherThanAttachRequestMessage(PHONE);
}

TEST_F(UeConnectionWithConnectedTransportTestSuite, shallAcceptAttachOnRequestFromUe)
{
    InSequence seq;
    EXPECT_CALL(*ueSlotNotAttachedMock, attach(PHONE)).WillOnce(Return(ueSlotAttachedMock));
    EXPECT_CALL(*transportMock, sendMessage(eqAttachResponseMessage(true)));

    handleAttachRequest(PHONE);
    ASSERT_EQ(PHONE, objectUnderTest->getPhoneNumber());
    ASSERT_TRUE(objectUnderTest->isAttached());
}

TEST_F(UeConnectionWithConnectedTransportTestSuite, shallRejectAttachOnRequestFromUeWithoutPhone)
{
    const PhoneNumber NO_PHONE{};

    EXPECT_CALL(*transportMock, sendMessage(eqAttachResponseMessage(false, NO_PHONE)));

    handleAttachRequest(NO_PHONE);
    ASSERT_EQ(NO_PHONE, objectUnderTest->getPhoneNumber());
    ASSERT_FALSE(objectUnderTest->isAttached());
}

TEST_F(UeConnectionWithConnectedTransportTestSuite, shallHandleExceptionWhenAttaching)
{
    EXPECT_CALL(*ueSlotNotAttachedMock, attach(PHONE)).WillOnce(Throw(std::runtime_error("..it happens")));
    handleAttachRequest(PHONE);
}

TEST_F(UeConnectionWithConnectedTransportTestSuite, shallIndicateUnknownSenderForOtherThanAttachRequest)
{
    auto otherThanAttachRequestMessage = buildOtherThanAttachRequestMessage();
    auto matchUnknownSenderMessage = AllOf(EqMessageHeader(0, MessageId::UnknownSender, NO_PHONE, NO_PHONE),
                                           EqMessageHeader(HEADER_SIZE, OTHER_THAN_ATTACH_REQUEST_MESSAGE, PHONE, OTHER_PHONE));
    EXPECT_CALL(*transportMock, sendMessage(matchUnknownSenderMessage));
    ueMessageCallback(otherThanAttachRequestMessage);
}

TEST_F(UeConnectionWithConnectedTransportTestSuite, shallPrintAsNotAttached)
{
    std::ostringstream os;
    os << *objectUnderTest;
    ASSERT_THAT(os.str(), AllOf(
                    Not(HasSubstr(to_string(PHONE))),
                    HasSubstr("I"), // not attached
                    HasSubstr(TRANSPORT_ADDRESS)
                    ));
}

UeConnectionAttachedTestSuite::UeConnectionAttachedTestSuite()
{
    UeConnectionWithConnectedTransportTestSuite::SetUp();
    EXPECT_CALL(*ueSlotNotAttachedMock, attach(PHONE)).WillOnce(Return(ueSlotAttachedMock));
    EXPECT_CALL(*transportMock, sendMessage(eqAttachResponseMessage(true)));
    handleAttachRequest(PHONE);
    verifyAndClearExpectations();
}

TEST_F(UeConnectionAttachedTestSuite, shallCloseConnectionOnDisconnect)
{
    EXPECT_CALL(*ueSlotAttachedMock, remove());
    handleDisconnect();
}

TEST_F(UeConnectionAttachedTestSuite, shallHandleExceptionWhenClosing)
{
    EXPECT_CALL(*ueSlotAttachedMock, remove()).WillOnce(Throw(std::runtime_error("..it happens")));
    handleDisconnect();
}

TEST_F(UeConnectionAttachedTestSuite, shallReattachOnRequestWithNewPhone)
{
    EXPECT_CALL(*ueSlotAttachedMock, attach(OTHER_PHONE)).WillOnce(Return(ueSlotReattachedMock));
    EXPECT_CALL(*transportMock, sendMessage(eqAttachResponseMessage(true, OTHER_PHONE)));

    handleAttachRequest(OTHER_PHONE);

    ASSERT_EQ(OTHER_PHONE, objectUnderTest->getPhoneNumber());
    ASSERT_TRUE(objectUnderTest->isAttached());
}

TEST_F(UeConnectionAttachedTestSuite, shallFailReattachOnRequestWithNewPhoneIfSlotDoesNotSucceedToReAttach)
{
    EXPECT_CALL(*ueSlotAttachedMock, attach(OTHER_PHONE)).WillOnce(Return(ueSlotFailedAttachedMock));
    EXPECT_CALL(*transportMock, sendMessage(eqAttachResponseMessage(false, OTHER_PHONE)));

    handleAttachRequest(OTHER_PHONE);

    ASSERT_FALSE(objectUnderTest->isAttached());
}

TEST_F(UeConnectionAttachedTestSuite, shallStayAttachedOnRequestWithSamePhone)
{
    EXPECT_CALL(*transportMock, sendMessage(eqAttachResponseMessage(true)));
    handleAttachRequest(PHONE);

    ASSERT_EQ(PHONE, objectUnderTest->getPhoneNumber());
    ASSERT_TRUE(objectUnderTest->isAttached());
}

TEST_F(UeConnectionAttachedTestSuite, shallForwardMessage)
{
    auto otherThanAttachRequestMessage = buildOtherThanAttachRequestMessage();
    auto matchMessage = Field(&BinaryMessage::value, ContainerEq(otherThanAttachRequestMessage.value));
    EXPECT_CALL(*ueSlotAttachedMock, sendMessage(matchMessage, OTHER_PHONE))
            .WillOnce(Return(true));
    ueMessageCallback(otherThanAttachRequestMessage);
}

TEST_F(UeConnectionAttachedTestSuite, shallIndicateUnknownRecipientForMessageThatCannotBeForwarded)
{
    auto otherThanAttachRequestMessage = buildOtherThanAttachRequestMessage();
    auto matchMessage = Field(&BinaryMessage::value, otherThanAttachRequestMessage.value);
    InSequence seq;
    EXPECT_CALL(*ueSlotAttachedMock, sendMessage(matchMessage, OTHER_PHONE))
            .WillOnce(Return(false));

    auto matchUnknownRecipientMessage = AllOf(EqMessageHeader(0, MessageId::UnknownRecipient, NO_PHONE, PHONE),
                                              EqMessageHeader(HEADER_SIZE, OTHER_THAN_ATTACH_REQUEST_MESSAGE, PHONE, OTHER_PHONE));
    EXPECT_CALL(*transportMock, sendMessage(matchUnknownRecipientMessage));
    ueMessageCallback(otherThanAttachRequestMessage);
}

TEST_F(UeConnectionAttachedTestSuite, shallIndicateUnknownSenderForMessageThatHasWrongFromPhone)
{
    auto otherThanAttachRequestMessageWithWrongFromPhone = buildOtherThanAttachRequestMessage(NOT_MY_PHONE);
    auto matchUnknownSenderMessage = AllOf(EqMessageHeader(0, MessageId::UnknownSender, NO_PHONE, PHONE),
                                           EqMessageHeader(HEADER_SIZE, OTHER_THAN_ATTACH_REQUEST_MESSAGE, NOT_MY_PHONE, OTHER_PHONE));
    EXPECT_CALL(*transportMock, sendMessage(matchUnknownSenderMessage));
    ueMessageCallback(otherThanAttachRequestMessageWithWrongFromPhone);
}

TEST_F(UeConnectionAttachedTestSuite, shallHandleExceptionWhenHandlingMessage)
{
    auto otherThanAttachRequestMessage = buildOtherThanAttachRequestMessage();
    auto matchMessage = Field(&BinaryMessage::value, otherThanAttachRequestMessage.value);
    InSequence seq;
    EXPECT_CALL(*ueSlotAttachedMock, sendMessage(matchMessage, OTHER_PHONE))
            .WillOnce(Throw(std::runtime_error("..it happens")));
    ueMessageCallback(otherThanAttachRequestMessage);
}

TEST_F(UeConnectionAttachedTestSuite, shallPrintAsAttached)
{
    std::ostringstream os;
    os << *objectUnderTest;
    ASSERT_THAT(os.str(), AllOf(
                    HasSubstr(to_string(PHONE)),
                    HasSubstr("A"), // attached
                    HasSubstr(TRANSPORT_ADDRESS)
                    ));
}

}
