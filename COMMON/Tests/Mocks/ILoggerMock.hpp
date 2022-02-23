#include <gmock/gmock.h>
#include "Logger/ILogger.hpp"

namespace common
{

struct ILoggerMock : public ILogger
{
    ILoggerMock();
    ~ILoggerMock() override;

    MOCK_METHOD(void, log, (ILogger::Level, const std::string& message), (final));
};

} // namespace common
