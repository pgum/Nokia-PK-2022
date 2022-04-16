#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "SMSDB/BasicSMSDatabase.hpp"
#include "SMSDB/SMS.hpp"
#include "Messages/PhoneNumber.hpp"


using namespace ::testing;

class BasicSMSDBTestSuite : public Test
{
protected:
    const common::PhoneNumber FROM[3] = {101, 102, 103};
    const common::PhoneNumber TO{201};
    const std::string MESSAGES[3] = {"Hello. How are you?", "Test message 2.", "Short msg"};
    const std::string SUMMARIES[3] = {"Hello. How are ", "Test message 2.", "Short msg"};

    ue::BasicSMSDatabase objectUnderTest{};
};

struct SingleRecordDBTestSuite : BasicSMSDBTestSuite
{
public:
    SingleRecordDBTestSuite();
};

SingleRecordDBTestSuite::SingleRecordDBTestSuite()
{
    objectUnderTest.addSMS(FROM[0], TO, MESSAGES[0]);
}

TEST_F(SingleRecordDBTestSuite, shallReturnAddedMessage)
{
    ASSERT_EQ(objectUnderTest.getSMS(0).getMessage(), MESSAGES[0]);
}

TEST_F(SingleRecordDBTestSuite, shallReturnAddedSourceNumber)
{
    ASSERT_EQ(objectUnderTest.getSMS(0).getFromNumber(), FROM[0]);
}

TEST_F(SingleRecordDBTestSuite, shallReturnAddedDestinationNumber)
{
    ASSERT_EQ(objectUnderTest.getSMS(0).getToNumber(), TO);
}
