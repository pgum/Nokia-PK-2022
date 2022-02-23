#pragma once

#include <functional>
#include <string>
#include <iostream>

namespace bts
{

class IConsole
{
public:
    using CommandCallback = std::function<void(std::string, std::ostream&)>;

    virtual ~IConsole() = default;

    virtual void addCommand(std::string command, const std::string &commandText, CommandCallback) = 0;

    /**
     * @brief Close command is the last invoked.
     */
    virtual void addCloseCommand(std::string commandLetter = "q", const std::string &commandText = "quit", CommandCallback = [](auto,auto&){}) = 0;

    /**
     * @brief Help command prints help about other commands
     */
    virtual void addHelpCommand(std::string commandLetter = "h", const std::string &commandText = "help") = 0;
};

}
