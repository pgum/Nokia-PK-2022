#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "SMSDB/BasicSMSDatabase.hpp"
#include "SMSDB/SMS.hpp"
#include "Messages/PhoneNumber.hpp"


using namespace ::testing;

class BasicSMSDBTestSuite : public Test
{
protected:
    const common::PhoneNumber FROM[2] = {101, 102};
    const common::PhoneNumber TO{201};
    const std::string MESSAGES[2] = {"Hello. How are you?", "Test message 2."};
    const std::string SUMMARIES[2] = {"Hello. How are ", "Test message 2."};

    ue::BasicSMSDatabase objectUnderTest{};
};

struct SingleRecordSMSDBTestSuite : BasicSMSDBTestSuite
{
public:
    SingleRecordSMSDBTestSuite();
};

SingleRecordSMSDBTestSuite::SingleRecordSMSDBTestSuite()
{
    objectUnderTest.addSMS(FROM[0], TO, MESSAGES[0]);
}

TEST_F(SingleRecordSMSDBTestSuite, shallReturnAddedMessage)
{
    ASSERT_EQ(objectUnderTest.getSMS(0).getMessage(), MESSAGES[0]);
}

TEST_F(SingleRecordSMSDBTestSuite, shallReturnAddedSourceNumber)
{
    ASSERT_EQ(objectUnderTest.getSMS(0).getFromNumber(), FROM[0]);
}

TEST_F(SingleRecordSMSDBTestSuite, shallReturnAddedDestinationNumber)
{
    ASSERT_EQ(objectUnderTest.getSMS(0).getToNumber(), TO);
}

TEST_F(SingleRecordSMSDBTestSuite, shallReturnVectorOfOneSummary)
{
    const std::vector<std::pair<unsigned int, std::unique_ptr< ITextMessage>>>& resultVector = objectUnderTest.getAllSMS();

    ASSERT_EQ(resultVector.size(),1);
    ASSERT_EQ(resultVector[0].second->getMessageSummary(),SUMMARIES[0]);
}

struct MultipleRecordsSMSDBTestSuite : BasicSMSDBTestSuite
{
protected:
    MultipleRecordsSMSDBTestSuite();
    const std::string REPLY_MESSAGE = "Reply.";
    const std::string REPLY_SUMMARY = "Reply.";
};

MultipleRecordsSMSDBTestSuite::MultipleRecordsSMSDBTestSuite()
{
    objectUnderTest.addSMS(FROM[0], TO, MESSAGES[0]);
    objectUnderTest.addSMS(FROM[1], TO, MESSAGES[1]);
    //reply to FROM[1] number
    objectUnderTest.addSMS(TO, FROM[1], REPLY_MESSAGE);
}

TEST_F(MultipleRecordsSMSDBTestSuite, shallReturnAddedMessages)
{
    ASSERT_EQ(objectUnderTest.getSMS(0).getMessage(), MESSAGES[0]);
    ASSERT_EQ(objectUnderTest.getSMS(1).getMessage(), MESSAGES[1]);

    ASSERT_EQ(objectUnderTest.getSMS(2).getMessage(), REPLY_MESSAGE);
}

TEST_F(MultipleRecordsSMSDBTestSuite, shallReturnAddedSourceNumbers)
{
    ASSERT_EQ(objectUnderTest.getSMS(0).getFromNumber(), FROM[0]);
    ASSERT_EQ(objectUnderTest.getSMS(1).getFromNumber(), FROM[1]);

    ASSERT_EQ(objectUnderTest.getSMS(2).getFromNumber(), TO);
}

TEST_F(MultipleRecordsSMSDBTestSuite, shallReturnAddedDestinationNumbers)
{
    ASSERT_EQ(objectUnderTest.getSMS(0).getToNumber(), TO);
    ASSERT_EQ(objectUnderTest.getSMS(1).getToNumber(), TO);

    ASSERT_EQ(objectUnderTest.getSMS(2).getToNumber(), FROM[1]);
}

TEST_F(MultipleRecordsSMSDBTestSuite, shallReturnVectorOfSummaries)
{
    const std::vector<std::pair<unsigned int, std::unique_ptr< ITextMessage>>>& resultVector = objectUnderTest.getAllSMS();

    ASSERT_EQ(resultVector.size(), 3);

    ASSERT_EQ(resultVector[0].second->getMessageSummary(), SUMMARIES[0]);
    ASSERT_EQ(resultVector[1].second->getMessageSummary(), SUMMARIES[1]);

    ASSERT_EQ(resultVector[2].second->getMessageSummary(), REPLY_SUMMARY);
}
