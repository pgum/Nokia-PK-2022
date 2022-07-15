#pragma once

#include "ITimerPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "thread"
#include "atomic"

namespace ue
{

    class TimerPort : public ITimerPort
    {
    public:
        TimerPort(common::ILogger& logger);

        void start(ITimerEventsHandler& handler);
        void stop();

        // ITimerPort interface
        void startTimer(Duration duration) override;
        void stopTimer() override;



    private:
        void timerRun(std::chrono::duration<double> timerDuration);
        std::thread timerThread;
        std::atomic<bool> timerRunning {false};
        common::PrefixedLogger logger;
        ITimerEventsHandler* handler = nullptr;
    };
}