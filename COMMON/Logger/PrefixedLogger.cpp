#include "PrefixedLogger.hpp"

namespace common
{

inline std::ostream& operator << (std::ostream& os, PrefixedLogger::Prefix const& prefix)
{
    prefix(os);
    return os;
}

PrefixedLogger::PrefixedLogger(ILogger& adaptee, Prefix prefix)
    : adaptee(adaptee), prefix(prefix)
{}

PrefixedLogger::PrefixedLogger(ILogger& adaptee, const std::string& prefix)
    : PrefixedLogger(adaptee, [this, prefix] (std::ostream& os){ os << prefix; })
{}

void PrefixedLogger::log(Level level, const std::string &message)
{
    adaptee.log(level, prefix, message);
}

} // namespace ue
