#pragma once

#include <thread>
#include <atomic>
#include <chrono>
#include "IComponent.hpp"
#include "Synchronization.hpp"
#include "UeRelay/IUeRelay.hpp"
#include "Messages/BtsId.hpp"
#include "Logger/PrefixedLogger.hpp"

namespace bts
{

class SibMolester : public IComponent
{
public:
    SibMolester(std::shared_ptr<IUeRelay> ueRelay,
                SyncGuardPtr syncGuard,
                BtsId btsId,
                common::ILogger& logger,
                std::chrono::milliseconds tickDuration = std::chrono::milliseconds(100),
                std::size_t ticksToSendSib = 50);
    ~SibMolester();

    void start() override;
    void stop() override;
private:
    void run();
    void oneTick();
    void oneSib();
    void sendSib();
    void sendSib(IUeConnection &ue);

    std::shared_ptr<IUeRelay> ueRelay;
    SyncGuardPtr syncGuard;
    common::PrefixedLogger logger;
    BtsId btsId;
    const std::chrono::milliseconds TICK_DURATION;
    const std::size_t TICKS_TO_SEND_SIB;

    std::size_t sibIndex = 0;
    std::size_t tickIndex = 0;
    std::atomic_bool running{false};
    std::thread molester;
};

}
