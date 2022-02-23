#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "UeRelay/UeRelay.hpp"

#include "Mocks/IUeConnectionMock.hpp"
#include "Mocks/ILoggerMock.hpp"

namespace bts
{

class UeRelayTestSuite : public ::testing::Test
{
protected:
    struct ConnectionMock;

    UeRelayTestSuite();
    void verifyAndClearExpectations();
    void shallForwardMessage(ConnectionMock& connnection);
    void shallNotForwardMessage(PhoneNumber phoneNumber);
    void expectAction(ConnectionMock& connnection);
    IUeRelay::UeVisitor getAction();

    struct ConnectionMock
    {
        ConnectionMock();

        IUeConnectionMock* connectionMock;
        IUeRelay::UePtr connectionPtr;
        UeSlot connectionSlot;
        PhoneNumber phoneNumber{};

        void add(IUeRelay& ueRelay);
        void attach(PhoneNumber phoneNumber);
        void remove();

        void expectSendMessage(const BinaryMessage& message);
        void expectSendSib(BtsId btsId);

        void printConnection(std::ostream &os);

        static constexpr std::size_t count() { return 1; }
    };


    ::testing::NiceMock<common::ILoggerMock> loggerMock{};
    const BtsId BTS_ID{17};
    const PhoneNumber ATTACHED_PHONE{123};
    const PhoneNumber ATTACHED_PHONE_2{25};
    const PhoneNumber REATTACHED_PHONE{32};
    const PhoneNumber NOT_ATTACHED_PHONE{111};
    const BinaryMessage MESSAGE{{1,2,3,4,5,6}};

    ConnectionMock connectionNotAdded;
    ConnectionMock connectionAdded;
    ConnectionMock connectionAttached;
    ConnectionMock connectionReAttached;

    std::unique_ptr<UeRelay> objectUnderTest;
};


}
