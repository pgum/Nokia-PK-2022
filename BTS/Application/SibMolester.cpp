#include "SibMolester.hpp"
#include <chrono>

namespace bts
{

SibMolester::SibMolester(std::shared_ptr<IUeRelay> ueRelay,
                         SyncGuardPtr syncGuard,
                         BtsId btsId,
                         common::ILogger &logger,
                         std::chrono::milliseconds oneTickDuration,
                         std::size_t ticksToSendSib)
    : ueRelay(ueRelay),
      syncGuard(syncGuard),
      btsId(btsId),
      logger(logger, "[SIB]"),
      TICK_DURATION(oneTickDuration),
      TICKS_TO_SEND_SIB(ticksToSendSib)
{}

SibMolester::~SibMolester()
{
    if (running)
    {
        logger.logError("running on destruction!");
    }
}

void SibMolester::start()
{
    if (false == running.exchange(true))
    {
        molester = std::thread(std::bind(&SibMolester::run, this));
    }
    else
    {
        logger.logError("attempt to restart!");
    }
}

void SibMolester::stop()
{
    if (true == running.exchange(false))
    {
        molester.join();
    }
    else
    {
        logger.logError("attempt to stop not running thread!");
    }
}

void SibMolester::run()
{
    logger.logDebug("started");
    while (running)
    {
        oneSib();
        oneTick();
    }
    logger.logDebug("finished");
}

void SibMolester::oneTick()
{
    std::this_thread::sleep_for(TICK_DURATION);
    ++tickIndex;
}

void SibMolester::sendSib(IUeConnection &ue)
{
    logger.logDebug("send to [", sibIndex, "]: ", ue);
    ue.sendSib(btsId);
}

void SibMolester::oneSib()
{
    if (tickIndex < TICKS_TO_SEND_SIB)
    {
        // not time yet!
        return;
    }
    tickIndex = 0;
    sendSib();
}

void SibMolester::sendSib()
{
    SyncLock lock(*syncGuard);
    auto notAttached = ueRelay->countNotAttached();
    if (notAttached == 0)
    {
        return;
    }
    if (sibIndex >= notAttached)
    {
        sibIndex = 0;
    }
    ueRelay->visitNotAttachedUe([this, i = 0, sent=false] (IUeConnection& ue) mutable
    {
        if (not sent && i == sibIndex)
        {
            sendSib(ue);
            ++sibIndex;
            sent = true;
        }
        ++i;
    });
}

}
