#pragma once

#include "IUeConnectionFactory.hpp"
#include "Logger/ILogger.hpp"
#include "Synchronization.hpp"

namespace bts
{

class UeConnectionFactory : public IUeConnectionFactory
{
public:
    UeConnectionFactory(common::ILogger& logger,
                        std::shared_ptr<SyncGuard> syncGuard);

    IUeRelay::UePtr createConnection(ITransportPtr transport) override;

private:
    std::shared_ptr<IUeRelay> ueRelay;
    common::ILogger& logger;
    std::shared_ptr<SyncGuard> syncGuard;
};

}
