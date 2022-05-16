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
    const std::string SUMMARIES_UNREAD[2] = {"*Hello. How are", "*Test message 2"};
    const std::string SUMMARIES_READ[2] = {"Hello. How are ", "Test message 2."};

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
    const std::vector<std::pair<unsigned int, std::shared_ptr< ue::ITextMessage>>>& resultVector = objectUnderTest.getAllSMS();

    ASSERT_EQ(resultVector.size(),1);
    ASSERT_EQ(resultVector[0].second->getMessageSummary(), SUMMARIES_UNREAD[0]);
}

TEST_F(SingleRecordSMSDBTestSuite, shallReturnReadSummaryAfterGetMessage)
{
    const std::vector<std::pair<unsigned int, std::shared_ptr< ue::ITextMessage>>>& resultVector = objectUnderTest.getAllSMS();

    ASSERT_EQ(resultVector.size(),1);
    ASSERT_EQ(resultVector[0].second->getMessageSummary(), SUMMARIES_UNREAD[0]);
    ASSERT_EQ(resultVector[0].second->getMessage(), MESSAGES[0]);
    resultVector[0].second->setIsReadStatus(true);
    ASSERT_EQ(resultVector[0].second->getMessageSummary(), SUMMARIES_READ[0]);
}

struct MultipleRecordsSMSDBTestSuite : BasicSMSDBTestSuite
{
protected:
    MultipleRecordsSMSDBTestSuite();
    const std::string REPLY_MESSAGE = "Reply. Testing long message.";
    const std::string REPLY_SUMMARY_READ = "Reply. Testing ";
};

MultipleRecordsSMSDBTestSuite::MultipleRecordsSMSDBTestSuite()
{
    objectUnderTest.addSMS(FROM[0], TO, MESSAGES[0]);
    objectUnderTest.addSMS(FROM[1], TO, MESSAGES[1]);
    //reply to FROM[1] number
    objectUnderTest.addSMS(TO, FROM[1], REPLY_MESSAGE,true);
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
    const std::vector<std::pair<unsigned int, std::shared_ptr< ue::ITextMessage>>>& resultVector = objectUnderTest.getAllSMS();

    ASSERT_EQ(resultVector.size(), 3);

    ASSERT_EQ(resultVector[0].second->getMessageSummary(), SUMMARIES_UNREAD[0]);
    ASSERT_EQ(resultVector[1].second->getMessageSummary(), SUMMARIES_UNREAD[1]);

    ASSERT_EQ(resultVector[2].second->getMessageSummary(), REPLY_SUMMARY_READ);
}

TEST_F(MultipleRecordsSMSDBTestSuite, shallReturnReadSummariesAfterGetMessage)
{
    const std::vector<std::pair<unsigned int, std::shared_ptr< ue::ITextMessage>>>& resultVector = objectUnderTest.getAllSMS();

    ASSERT_EQ(resultVector.size(), 3);

    ASSERT_EQ(resultVector[0].second->getMessageSummary(), SUMMARIES_UNREAD[0]);
    ASSERT_EQ(resultVector[1].second->getMessageSummary(), SUMMARIES_UNREAD[1]);

    ASSERT_EQ(resultVector[2].second->getMessageSummary(), REPLY_SUMMARY_READ);

    ASSERT_EQ(resultVector[0].second->getMessage(), MESSAGES[0]);
    ASSERT_EQ(resultVector[1].second->getMessage(), MESSAGES[1]);

    resultVector[0].second->setIsReadStatus(true);
    resultVector[1].second->setIsReadStatus(true);

    ASSERT_EQ(resultVector[0].second->getMessageSummary(), SUMMARIES_READ[0]);
    ASSERT_EQ(resultVector[1].second->getMessageSummary(), SUMMARIES_READ[1]);

}
