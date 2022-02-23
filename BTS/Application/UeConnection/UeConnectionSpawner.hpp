#pragma once

#include "IApplicationEnvironment.hpp"
#include "UeRelay/IUeRelay.hpp"
#include "UeConnection/IUeConnectionFactory.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Synchronization.hpp"
#include "IComponent.hpp"

namespace bts
{

class UeConnectionSpawner : public IComponent
{
public:
    UeConnectionSpawner(IApplicationEnvironment& environment,
                        std::shared_ptr<IUeConnectionFactory> ueConnectionFactory,
                        std::shared_ptr<IUeRelay> ueRelay,
                        SyncGuardPtr syncGuard);
    ~UeConnectionSpawner();

    void start() override;
    void stop() override;

private:
    void spawnConnection(ITransportPtr transport);

    IApplicationEnvironment& environment;
    std::shared_ptr<IUeConnectionFactory> ueConnectionFactory;
    std::shared_ptr<IUeRelay> ueRelay;
    common::PrefixedLogger logger;
    BtsId btsId;
    SyncGuardPtr syncGuard;
};

}
