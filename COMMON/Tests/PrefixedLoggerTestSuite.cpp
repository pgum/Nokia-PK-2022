#include <gtest/gtest.h>
#include <gmock/gmock.h>


#include "Logger/PrefixedLogger.hpp"
#include "Mocks/ILoggerMock.hpp"

namespace common
{

using namespace ::testing;

class PrefixedLoggerTestSuite : public Test
{
protected:
    const std::string message1 = "What a mess!";
    const std::string message2 = "Nice, but not enough!";

    StrictMock<ILoggerMock> adapteeMock;
    const std::string prefix = "Something not empty";
    PrefixedLogger objectUnderTest{adapteeMock, prefix};
};

TEST_F(PrefixedLoggerTestSuite, shallPrintPrefixToDebugStream)
{
    auto matchPrefixAndMessage = AllOf(HasSubstr(message1), HasSubstr(prefix));
    EXPECT_CALL(adapteeMock, log(ILogger::DEBUG_LEVEL, matchPrefixAndMessage));
    objectUnderTest.logDebug(message1);
}

TEST_F(PrefixedLoggerTestSuite, shallPrintPrefixToInfoStream)
{
    auto matchPrefixAndMessage = AllOf(HasSubstr(message1), HasSubstr(prefix));
    EXPECT_CALL(adapteeMock, log(ILogger::INFO_LEVEL, matchPrefixAndMessage));
    objectUnderTest.logInfo(message1);
}

TEST_F(PrefixedLoggerTestSuite, shallPrintPrefixToErrorStream)
{
    auto matchPrefixAndMessage = AllOf(HasSubstr(message2), HasSubstr(prefix));
    EXPECT_CALL(adapteeMock, log(ILogger::ERROR_LEVEL, matchPrefixAndMessage));
    objectUnderTest.logError(message2);
}

} // namespace common
