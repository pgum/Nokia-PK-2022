#include "TimerPort.hpp"

namespace ue
{

    TimerPort::TimerPort(common::ILogger &logger)
            : logger(logger, "[TIMER PORT]")
    {}


    void TimerPort::start(ITimerEventsHandler &handler)
    {
        logger.logDebug("Started");
        this->handler = &handler;
        timerRunning.store(true);

    }

    void TimerPort::stop()
    {
        logger.logDebug("Stopped");
        timerRunning.store(false);
        handler = nullptr;
    }

    void TimerPort::startTimer(Duration duration)
    {
        if(timerRunning){
            timerRunning.store(false);
            return;
        }
        timerRunning.store(true);
        logger.logDebug("Start timer: ", duration.count(), "ms");

        timerThread = std::thread(&TimerPort::timerRun, this, duration);
        timerThread.detach();
    }

    void TimerPort::stopTimer()
    {
        logger.logDebug("Stop timer");
        timerRunning.store(false);
    }

    void TimerPort::timerRun(std::chrono::duration<double> timerDuration){

        auto start = std::chrono::system_clock::now();

        while((std::chrono::system_clock::now() - timerDuration) < start){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(!timerRunning){
                return;
            }
        }
        timerRunning.store(false);
        handler->handleTimeout();
    }

}