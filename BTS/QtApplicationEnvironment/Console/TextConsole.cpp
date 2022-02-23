#include "TextConsole.hpp"
#include <algorithm>
#include <sstream>
#include <regex>

namespace bts
{

TextConsole::TextConsole(common::ILogger &logger)
    : logger(logger, "[Console]")
{

}

void TextConsole::addCommand(std::string command, const std::string &commandText, IConsole::CommandCallback commandCallback)
{
    logger.logDebug("Add: `", command, "` for \"", commandText, "\"");
    commands.push_back({command, commandText, commandCallback});
}

void TextConsole::addCloseCommand(std::string command, const std::string &commandText, IConsole::CommandCallback commandCallback)
{
    logger.logDebug("Add close: `", command, "` for \"", commandText, "\"");
    auto closeLoop = [this, commandCallback](std::string args, std::ostream& os)
    {
        isRunning = false;
        commandCallback(args, os);
        logger.logDebug("Console closed!");
    };
    commands.push_back({command, commandText, closeLoop});
}

void TextConsole::addHelpCommand(std::string command, const std::string &commandText)
{
    logger.logDebug("Add help: `", command, "` for \"", commandText, "\"");
    auto helpCommand = [this](std::string, std::ostream& os)
    {
        printHelp(os);
    };
    commands.push_back({command, commandText, helpCommand});
}

TextConsole::CommandLine TextConsole::getCommandLine()
{
    do
    {
        std::string line;
        std::getline(std::cin, line);
        if (line.empty())
        {
            continue;
        }

        TextConsole::CommandLine commandLine;
        std::istringstream iss(line);
        iss >> commandLine.command;
        if (commandLine.command.empty())
        {
            continue;
        }
        commandLine.args = readArgs(iss);
        return commandLine;

    } while (true);
}

std::string TextConsole::readArgs(std::istream& is)
{
    std::string args;
    std::getline(is, args);
    args = std::regex_replace(args, std::regex("^\\s+"), "");
    args = std::regex_replace(args, std::regex("\\s+$"), "");
    return args;
}


IConsole::CommandCallback TextConsole::getCallback(std::string commandText) const
{
    auto commandMatch = [commandText](const Command& command){ return commandText == command.command; };
    auto it = std::find_if(commands.begin(), commands.end(), commandMatch);
    if (it != commands.end())
    {
        return it->callback;
    }
    else
    {
        std::cout << commandText << ": not recognized!" << std::endl;
        return {};
    }
}

void TextConsole::run()
{
    printHelp(std::cout);
    while (isRunning)
    {
        CommandLine commandLine = getCommandLine();
        auto callback = getCallback(commandLine.command);
        if (callback)
        {
            callback(commandLine.args, std::cout);
        }
        else
        {
            printHelp(std::cout);
        }
        std::cout << std::endl;
    }
    emit quit();
}

void TextConsole::printHelp(std::ostream &os)
{
    for (Command const& c: commands)
    {
        os << c.command << ": " << c.description << std::endl;
    }
}

}
