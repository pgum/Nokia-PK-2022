#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Application.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include <memory>

namespace ue
{
using namespace ::testing;

class ApplicationTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{112};
    const common::BtsId BTS_ID{42};
    NiceMock<common::ILoggerMock> loggerMock;
    NiceMock<IBtsPortMock> btsPortMock;
    NiceMock<IUserPortMock> userPortMock;
    NiceMock<ITimerPortMock> timerPortMock;

    Application objectUnderTest{PHONE_NUMBER,
                                loggerMock,
                                btsPortMock,
                                userPortMock,
                                timerPortMock};
};

struct ApplicationNotConnectedTestSuite : ApplicationTestSuite
{
    void requestAttachOnSib();
};

void ApplicationNotConnectedTestSuite::requestAttachOnSib() {
    EXPECT_CALL(btsPortMock, sendAttachRequest(BTS_ID));
    EXPECT_CALL(timerPortMock, startTimer(_));
    EXPECT_CALL(userPortMock, showConnecting());
    objectUnderTest.handleSib(BTS_ID);
}

TEST_F(ApplicationNotConnectedTestSuite, shallRequestAttachOnSib)
{
    requestAttachOnSib();
}

struct ApplicationConnectingTestSuite : ApplicationNotConnectedTestSuite
{
    ApplicationConnectingTestSuite();
};

ApplicationConnectingTestSuite::ApplicationConnectingTestSuite() {
    requestAttachOnSib();
}

TEST_F(ApplicationConnectingTestSuite, shallCompleteAttachWhenAttachAccepted)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleAttachAccept();
}

TEST_F(ApplicationConnectingTestSuite, shallFailAttachWhenAttachRejected) {
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showNotConnected());
    objectUnderTest.handleAttachReject();
}

TEST_F(ApplicationConnectingTestSuite, shallFailAttachOnTimeout) {
    EXPECT_CALL(userPortMock, showNotConnected());
    objectUnderTest.handleTimeout();
}

struct ApplicationConnectedTestSuite : ApplicationConnectingTestSuite
{
    ApplicationConnectedTestSuite();
};

ApplicationConnectedTestSuite::ApplicationConnectedTestSuite() {
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleAttachAccept();
}

TEST_F(ApplicationConnectedTestSuite, shallReturnPhoneNumber) {
    common::PhoneNumber number {000};
//    EXPECT_CALL(userPortMock, showConnected());
    EXPECT_EQ(btsPortMock.getOwnPhoneNumber(), number);
}

TEST_F(ApplicationConnectedTestSuite, shallShowSms) {
    // TODO: sms test
    EXPECT_CALL(userPortMock, getSmsDb());
    EXPECT_CALL(userPortMock, showSms(0));
//    objectUnderTest.handleSmsReceive(0, "SMS_TEST", PHONE_NUMBER, PHONE_NUMBER);
}

TEST_F(ApplicationConnectedTestSuite, shallShowSmsList) {
    // TODO: sms list test
    EXPECT_CALL(userPortMock, showSmsList());
}

TEST_F(ApplicationConnectedTestSuite, shallHandleDisconnect) {
    EXPECT_CALL(userPortMock, showNotConnected());
    objectUnderTest.handleDisconnected();
}

}
