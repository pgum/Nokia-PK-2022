#include "UeConnectionSpawner.hpp"

namespace bts
{

using namespace std::placeholders;

UeConnectionSpawner::UeConnectionSpawner(IApplicationEnvironment& environment,
                                         std::shared_ptr<IUeConnectionFactory> ueConnectionFactory,
                                         std::shared_ptr<IUeRelay> ueRelay,
                                         SyncGuardPtr syncGuard)
    : environment(environment),
      ueConnectionFactory(ueConnectionFactory),
      ueRelay(ueRelay),
      logger(environment.getLogger(), "[SPAWNER]"),
      btsId(environment.getBtsId()),
      syncGuard(syncGuard)
{}

UeConnectionSpawner::~UeConnectionSpawner()
{}

void UeConnectionSpawner::start()
{
    logger.logDebug("Listen to new connections");
    SyncLock lock(*syncGuard);
    environment.registerUeConnectedCallback(std::bind(&UeConnectionSpawner::spawnConnection, this, std::placeholders::_1));
}

void UeConnectionSpawner::stop()
{
    logger.logDebug("Stop listenning to new connections");
    SyncLock lock(*syncGuard);
    environment.registerUeConnectedCallback(nullptr);
}


void UeConnectionSpawner::spawnConnection(ITransportPtr transport)
{
    logger.logDebug("new connection: ", transport->addressToString());
    auto newUe = ueConnectionFactory->createConnection(transport);
    auto* newUePtr = newUe.get();

    SyncLock lock(*syncGuard);
    auto ueSlot = ueRelay->add(std::move(newUe));
    newUePtr->start(ueSlot);
    newUePtr->sendSib(btsId);
}


}
