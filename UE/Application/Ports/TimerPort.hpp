#pragma once

#include "ITimerPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include <chrono>
#include <thread>

namespace ue {

    class TimerPort : public ITimerPort {
    public:
        TimerPort(common::ILogger &logger);

        void start(ITimerEventsHandler &handler);

        void stop();

        // ITimerPort interface
        void startTimer(Duration duration) override;
//        void startTimerAndDoSomething(std::function<void()>,double duration) override;
        void stopTimer() override;

    private:
        common::PrefixedLogger logger;
        ITimerEventsHandler *handler = nullptr;
        std::chrono::steady_clock::time_point beginning;
        std::chrono::steady_clock::time_point ending;
    };

}
