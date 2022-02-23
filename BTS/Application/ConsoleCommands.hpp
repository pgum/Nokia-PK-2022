#pragma once

#include "Synchronization.hpp"
#include "IConsole.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "UeRelay/IUeRelay.hpp"
#include "IApplicationEnvironment.hpp"
#include "IComponent.hpp"

namespace bts
{

class ConsoleCommands : public IComponent
{
public:
    ConsoleCommands(IConsole& console,
                    IApplicationEnvironment& environment,
                    common::ILogger& logger,
                    std::shared_ptr<IUeRelay> ueRelay,
                    SyncGuardPtr syncGuard);
    ~ConsoleCommands();

    void start() override;
    void stop() override;

private:
    void showAddress(std::string args, std::ostream &os);
    void showStatus(std::string args, std::ostream &os);
    void listAttachedUe(std::string args, std::ostream &os);
    void testCommands(std::string args, std::ostream &os);

    SyncGuardPtr syncGuard;
    common::PrefixedLogger logger;
    IConsole& console;
    IApplicationEnvironment& environment;
    std::shared_ptr<IUeRelay> ueRelay;
};

}
