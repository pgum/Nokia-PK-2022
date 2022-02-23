#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sstream>
#include <algorithm>

#include "Logger/Logger.hpp"

namespace common
{

using namespace ::testing;

class LoggerTestSuite : public TestWithParam<ILogger::Level>
{
protected:
    const std::string message1 = "That's not funny!";
    const std::string message2 = "That was pretty cool!";

    using LoggerFunction = void (ILogger::*)(const std::string&);
    struct LevelDefinition
    {
        LoggerFunction function;
        std::string severityStr;
        std::ostringstream log1{}, log2{};
    };

    LevelDefinition debugLevel = { &ILogger::logDebug<const std::string&>, "[DEBUG]" };
    LevelDefinition infoLevel = { &ILogger::logInfo<const std::string&>, "" };
    LevelDefinition errorLevel = { &ILogger::logError<const std::string&>, "[ERROR]" };

    std::vector<LevelDefinition*> levels{ &debugLevel, &infoLevel, &errorLevel };

    Logger objectUnderTest{
        { debugLevel.severityStr, { &debugLevel.log1, &debugLevel.log2} },
        { infoLevel.severityStr, { &infoLevel.log1, &infoLevel.log2} },
        { errorLevel.severityStr, { &errorLevel.log1, &errorLevel.log2} }
    };

    LevelDefinition& getParam()
    {
        return *levels.at(GetParam());
    }
    std::string getLog1()
    {
        return getParam().log1.str();
    }
    std::string getLog2()
    {
        return getParam().log2.str();
    }

    void printLog(const std::string& message)
    {
        auto function = getParam().function;
        (objectUnderTest.*function)(message);
    }
};

INSTANTIATE_TEST_SUITE_P(LoggerFunctions,
                        LoggerTestSuite,
                        Values(ILogger::DEBUG_LEVEL,
                               ILogger::INFO_LEVEL,
                               ILogger::ERROR_LEVEL));

TEST_P(LoggerTestSuite, shallPrintMessageToStreams)
{
    printLog(message1);
    ASSERT_THAT(getLog1(), HasSubstr(message1));
    ASSERT_THAT(getLog2(), HasSubstr(message1));
}

TEST_P(LoggerTestSuite, shallPrintIndeticalLogForAllStreams)
{
    printLog(message2);
    ASSERT_EQ(getLog1(), getLog2());
}

TEST_P(LoggerTestSuite, shallPrintSeverityToStreams)
{
    printLog(message2);
    ASSERT_THAT(getLog1(), HasSubstr(getParam().severityStr));
}

TEST_P(LoggerTestSuite, shallPrintMultipleLinesForMultipleLogsToStreams)
{
    printLog(message1);
    printLog(message2);
    const auto str = getLog1();
    ASSERT_THAT(str, HasSubstr(message1));
    ASSERT_THAT(str, HasSubstr(message2));
    ASSERT_EQ(2, std::count(str.begin(), str.end(), '\n'));
}

} // namespace common
