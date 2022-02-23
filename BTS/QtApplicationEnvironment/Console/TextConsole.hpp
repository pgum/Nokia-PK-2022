#pragma once

#include <vector>
#include <QObject>

#include "IConsole.hpp"
#include "Logger/ILogger.hpp"
#include "Logger/PrefixedLogger.hpp"

namespace bts
{
class TextConsole : public QObject, public IConsole
{
    Q_OBJECT;
public:
    TextConsole(common::ILogger& logger);
    void addCommand(std::string command, const std::string &commandText, CommandCallback commandCallback) override;
    void addCloseCommand(std::string command, const std::string &commandText, CommandCallback commandCallback) override;
    void addHelpCommand(std::string command, const std::string &commandText) override;

    void run();

signals:
    void quit();

private:
    common::PrefixedLogger logger;
    struct Command
    {
        std::string command;
        std::string description;
        CommandCallback callback;
    };
    struct CommandLine
    {
        std::string command;
        std::string args;
    };

    std::vector<Command> commands;

    void printHelp(std::ostream& os);
    void printCommand(const Command&);
    static CommandLine getCommandLine();
    static std::string readArgs(std::istream &is);
    IConsole::CommandCallback getCallback(std::string commandText) const;

    bool isRunning = true;
};

}

