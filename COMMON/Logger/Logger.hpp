#pragma once

#include "ILogger.hpp"
#include <mutex>
#include <atomic>
#include <vector>
#include <initializer_list>

namespace common
{

class Logger : public ILogger
{
public:
    struct LevelInfo
    {
        std::string prefix;
        std::vector<std::ostream*> streams;
    };

    Logger(std::ostream& logfile);
    Logger(std::initializer_list<LevelInfo> streamsForLevels);
    ~Logger() override;

    void log(Level level, const std::string& message) override;

private:
    std::vector<LevelInfo> streamsForLevels;
    std::mutex printoutGuard;
    std::atomic_size_t printoutNumber{};
};

} // namespace ue
