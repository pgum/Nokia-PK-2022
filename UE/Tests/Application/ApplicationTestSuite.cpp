#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Application.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Mocks/ISMSDatabaseMock.hpp"
#include "Mocks/IUeGuiMock.hpp"
#include <memory>

namespace ue
{
using namespace ::testing;

class ApplicationTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{112};
    const common::PhoneNumber NUMBER{101};
    const common::BtsId BTS_ID{42};
    const common::PhoneNumber SENDER_NUMBER{111};
    const std::string MESSAGE = "Hello there!";
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IBtsPortMock> btsPortMock;
    StrictMock<IUserPortMock> userPortMock;
    StrictMock<ITimerPortMock> timerPortMock;
    NiceMock<ISMSDatabaseMock> smsDb;

    Expectation expectNoTConnected = EXPECT_CALL(userPortMock, showNotConnected());

    Application objectUnderTest{PHONE_NUMBER,
                                loggerMock,
                                btsPortMock,
                                userPortMock,
                                timerPortMock,
                                smsDb};
};




struct ApplicationNotConnectedTestSuite : ApplicationTestSuite
{
    void requestAttachOnSib();
};

void ApplicationNotConnectedTestSuite::requestAttachOnSib()
{
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
    void requestConnectOnAttachRequest();
};

void ApplicationConnectingTestSuite::requestConnectOnAttachRequest()
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showMainMenu());
    EXPECT_CALL(userPortMock, setAcceptCallback(_));
    EXPECT_CALL(userPortMock, setRejectCallback(_));

    objectUnderTest.handleAttachAccept();
}

ApplicationConnectingTestSuite::ApplicationConnectingTestSuite()
{
    //preparation
    requestAttachOnSib();
}




TEST_F(ApplicationConnectingTestSuite, shallCompleteAttachWhenAttachAccepted)
{
    requestConnectOnAttachRequest();
}

TEST_F(ApplicationConnectingTestSuite, shallFaillAttachWhenAttachRejected)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showNotConnected());

    objectUnderTest.handleAttachReject();
}

TEST_F(ApplicationConnectingTestSuite, shallFailAttachOnTimeOut)
{
    EXPECT_CALL(userPortMock, showNotConnected());

    objectUnderTest.handleTimeout();
}





struct ApplicationConnectedTestSuite : ApplicationConnectingTestSuite
{
    ApplicationConnectedTestSuite();
    void testHandleCallRequest();
};

ApplicationConnectedTestSuite::ApplicationConnectedTestSuite()
{
    //preparation
    requestConnectOnAttachRequest();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleDisconnect)
{
    EXPECT_CALL(userPortMock, showNotConnected());

    objectUnderTest.handleDisconnected();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleReceivingSMS)
{
    EXPECT_CALL(userPortMock, showSMSNotification());
    EXPECT_CALL(userPortMock, getPhoneNumber());
    EXPECT_CALL(smsDb, addSMS(SENDER_NUMBER,_,MESSAGE,false,Received));

    objectUnderTest.handleSMS(SENDER_NUMBER,MESSAGE,common::MessageId::Sms);
}

void ApplicationConnectedTestSuite::testHandleCallRequest()
{
    using namespace std::chrono_literals;
    EXPECT_CALL(userPortMock, showNewCallRequest(NUMBER));
    EXPECT_CALL(timerPortMock, startTimer(30000ms));
    EXPECT_CALL(userPortMock, setAcceptCallback(_));
    EXPECT_CALL(userPortMock, setRejectCallback(_));
    objectUnderTest.handleCallRequest(NUMBER);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallRequest)
{
    testHandleCallRequest();
}





}