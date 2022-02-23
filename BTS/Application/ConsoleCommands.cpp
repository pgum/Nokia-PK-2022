#include "ConsoleCommands.hpp"
#include "TestCommands/TestCommands.hpp"

namespace bts
{

ConsoleCommands::ConsoleCommands(IConsole& console,
                                 IApplicationEnvironment &environment,
                                 common::ILogger& logger,
                                 std::shared_ptr<IUeRelay> ueRelay,
                                 SyncGuardPtr syncGuard)
    : syncGuard(syncGuard),
      logger(logger, "[CONSOLE]"),
      console(console),
      environment(environment),
      ueRelay(ueRelay)
{}

ConsoleCommands::~ConsoleCommands()
{}

void ConsoleCommands::start()
{
    SyncLock lock(*syncGuard);
    auto argsArgument = std::placeholders::_1;
    auto streamArgument = std::placeholders::_2;
    console.addCommand("a", "Show address", std::bind(&ConsoleCommands::showAddress, this, argsArgument, streamArgument));
    console.addCommand("s", "Show status", std::bind(&ConsoleCommands::showStatus, this, argsArgument, streamArgument));
    console.addCommand("l", "List attached ue", std::bind(&ConsoleCommands::listAttachedUe, this, argsArgument, streamArgument));
    console.addCloseCommand();
    console.addHelpCommand();
    console.addCommand("t", "Test commands - details in implementation",std::bind(&ConsoleCommands::testCommands, this, argsArgument, streamArgument));
}

void ConsoleCommands::stop()
{
    // do nothing
}

void ConsoleCommands::showAddress(std::string, std::ostream &os)
{
    SyncLock lock(*syncGuard);
    os << "BTS ID: " << environment.getBtsId() << "\n";
    os << "Address: " << environment.getAddress() << "\n";
}

void ConsoleCommands::showStatus(std::string, std::ostream& os)
{
    SyncLock lock(*syncGuard);
    os << "Connections: \n";
    os << " > ue attached: " << ueRelay->countAttached() << "\n";
    os << " > ue not attached: " << ueRelay->countNotAttached() << "\n";
}

void ConsoleCommands::listAttachedUe(std::string, std::ostream& os)
{
    SyncLock lock(*syncGuard);

    os << "attached ue: \n";
    ueRelay->visitAttachedUe([&os, i = 0](IUeConnection const& ue) mutable
    {
        os << "\t#" << ++i << ": " << ue << "\n";
    });
}

void ConsoleCommands::testCommands(std::string args, std::ostream &os)
{
    using common::TestCommands;
    try
    {
        TestCommands testParser(args);
        TestCommands::Parameters parameters{};
        parameters.sendMessage = [this] (BinaryMessage message,
                                         PhoneNumber to)
        {
            SyncLock lock(*syncGuard);
            ueRelay->sendMessage(message, to);
        };
        parameters.printText = [this, &os] (std::string message)
        {
            SyncLock lock(*syncGuard);
            os << message;
        };

        SyncLock lock(*syncGuard);
        testParser.run(parameters);
    }
    catch (std::exception& ex)
    {
        SyncLock lock(*syncGuard);
        os << " test commands syntax error: " << ex.what();
    }
}

}
