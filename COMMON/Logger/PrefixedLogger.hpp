#pragma once

#include "ILogger.hpp"
#include <functional>

namespace common
{

class PrefixedLogger : public ILogger
{
public:
    using Prefix = std::function<void(std::ostream&)>;
    PrefixedLogger(ILogger& adaptee, Prefix prefix);
    PrefixedLogger(ILogger& adaptee, const std::string& prefix);

    void log(Level level, const std::string& message) override;

private:
    ILogger& adaptee;
    Prefix prefix;
};

} // namespace ue
