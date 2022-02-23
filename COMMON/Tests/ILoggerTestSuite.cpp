#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sstream>

#include "Logger/ILogger.hpp"
#include "Mocks/ILoggerMock.hpp"

namespace common
{

using namespace ::testing;

using ILoggerTestable = ILoggerMock;

class ILoggerTestSuite : public Test
{
protected:
    const std::string message1 = "Have fun!";
    const int number1 = 123;
    const std::string message2 = "Back to work!";
    const float number2 = 1.23;
    const Matcher<std::string> matchNumber2 = HasSubstr("1.23"); // std::to_string(float) gives wrong result...

    StrictMock<ILoggerTestable> objectUnderTest;
};


TEST_F(ILoggerTestSuite, shallPrintValuesToDebugStream)
{
    auto matchPrintout = AllOf(HasSubstr(message1),
                               HasSubstr(std::to_string(number1)),
                               HasSubstr(message2),
                               matchNumber2);
    EXPECT_CALL(objectUnderTest, log(ILogger::DEBUG_LEVEL, matchPrintout));
    objectUnderTest.logDebug(message1, number1, message2, number2);
}

TEST_F(ILoggerTestSuite, shallPrintValuesToInfoStream)
{
    auto matchPrintout = AllOf(HasSubstr(message1),
                               HasSubstr(std::to_string(number1)));
    EXPECT_CALL(objectUnderTest, log(ILogger::INFO_LEVEL, matchPrintout));
    objectUnderTest.logInfo(message1, number1);
}

TEST_F(ILoggerTestSuite, shallPrintValuesToErrorStream)
{
    auto matchPrintout = AllOf(HasSubstr(message2),
                               matchNumber2);
    EXPECT_CALL(objectUnderTest, log(ILogger::ERROR_LEVEL, matchPrintout));
    objectUnderTest.logError(message2, number2);
}

} // namespace common
