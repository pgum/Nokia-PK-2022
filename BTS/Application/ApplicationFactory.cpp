#include "ApplicationFactory.hpp"
#include "Application.hpp"
#include "SibMolester.hpp"
#include "UeConnection/UeConnectionFactory.hpp"
#include "UeConnection/UeConnectionSpawner.hpp"
#include "UeRelay/UeRelay.hpp"
#include "ConsoleCommands.hpp"

namespace bts
{

std::unique_ptr<IComponent> createApplication(IApplicationEnvironment& environment)
{
    auto syncGuard = std::make_shared<SyncGuard>();
    auto& logger = environment.getLogger();

    auto ueRelay = std::make_shared<UeRelay>(environment.getLogger());
    auto ueConnectionFactory = std::make_shared<UeConnectionFactory>(environment.getLogger(), syncGuard);
    auto ueConnectionSpawner = std::make_shared<UeConnectionSpawner>(environment, ueConnectionFactory, ueRelay, syncGuard);
    auto sibMolester = std::make_shared<SibMolester>(ueRelay, syncGuard, environment.getBtsId(), environment.getLogger());
    auto consoleCommands = std::make_shared<ConsoleCommands>(environment.getConsole(), environment, environment.getLogger(), ueRelay, syncGuard);
    std::initializer_list<std::shared_ptr<IComponent>> components = {ueConnectionSpawner, sibMolester, consoleCommands};
    return std::make_unique<Application>(environment.getLogger(), components);
}

}
