#include "UeConnectionFactory.hpp"
#include "UeConnection.hpp"


namespace bts
{

UeConnectionFactory::UeConnectionFactory(common::ILogger &logger, std::shared_ptr<SyncGuard> syncGuard)
    : logger(logger),
      syncGuard(syncGuard)
{}

IUeRelay::UePtr UeConnectionFactory::createConnection(ITransportPtr transport)
{
    return std::make_unique<UeConnection>(transport, logger, syncGuard);
}

}
