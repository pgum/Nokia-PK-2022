#include "TimerPort.hpp"

namespace ue {

    TimerPort::TimerPort(common::ILogger &logger)
            : logger(logger, "[TIMER PORT]") {}

    void TimerPort::start(ITimerEventsHandler &handler) {
        logger.logDebug("Started");
        this->handler = &handler;
    }

    void TimerPort::stop() {
        logger.logDebug("Stoped");
        handler = nullptr;
    }

    void TimerPort::startTimer(Duration duration) {
        logger.logDebug("Start timer: ", duration.count(), "ms");
    }
    void TimerPort::startTimerAndDoSomething(std::function<void()> function,double duration)
    {
        logger.logDebug("Start timer: ", duration*1000, "ms");
        this->active=true;
        std::thread t([=]() {
            if(!this->active) return;
            int temp = (int)(duration*1000);
            std::this_thread::sleep_for(std::chrono::milliseconds(temp));
            if(!this->active) return;
            function();
        });
        t.detach();
    }

    void TimerPort::stopTimer() {
        logger.logDebug("Stop timer");
    }

}
