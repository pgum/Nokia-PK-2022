#include "TestCommands.hpp"
#include <sstream>
#include <stdexcept>
#include <chrono>
#include <thread>
#include "Messages/OutgoingMessage.hpp"
#include "Messages/MessageId.hpp"

namespace common
{

const TestCommands::TextCommandMap TestCommands::textCommandMap =
{

    {"repeat", &TestCommands::readRepeatCommand },
    {"r", &TestCommands::readRepeatCommand },

    {"send", &TestCommands::readSendCommand },
    {"s", &TestCommands::readSendCommand },

    {"wait", &TestCommands::readWaitCommand },
    {"w", &TestCommands::readWaitCommand },

    {"group", &TestCommands::readGroupCommand },
    {"g", &TestCommands::readGroupCommand },

    {"thread", &TestCommands::readThreadCommand },
    {"t", &TestCommands::readThreadCommand },

    {"write", &TestCommands::readWriteCommand },
    {"text", &TestCommands::readWriteCommand },
    {"echo", &TestCommands::readWriteCommand },
    {"e", &TestCommands::readWriteCommand },

};

TestCommands::TestCommands(std::string args)
{
    std::istringstream iss(args);
    while (Command command = readCommand(iss))
    {
        commands.push_back(command);
    }
}

void TestCommands::run(Parameters parameters)
{
    for (auto&& command : commands)
    {
        command(parameters);
    }
}

TestCommands::Command TestCommands::readCommand(std::istream &is)
{
    std::string commandText;

    if (not (is >> commandText))
    {
        return Command{};
    }

    auto it = textCommandMap.find(commandText);
    if (it == textCommandMap.end())
    {
        throwError("Not recognized command: \"" + commandText + "\"");
    }

    return (this->*(it->second))(is);
}

TestCommands::Command TestCommands::readRepeatCommand(std::istream &is)
{
    unsigned howMany = readArg<unsigned>(is, "'repeat' needs how-many-times number");
    Command subCommand = readCommand(is);
    if (not subCommand)
    {
        throwError("'repeat' needs sub command!");
    }
    return [howMany, subCommand](Parameters parameters)
    {
        for (unsigned i = 0; i < howMany; ++i)
        {
            subCommand(parameters);
        }
    };
}

TestCommands::Command TestCommands::readGroupCommand(std::istream &is)
{
    unsigned howMany = readArg<unsigned>(is, "'group' needs how-many-commands number");
    Commands subCommands;
    for (unsigned i = 0; i < howMany; ++i)
    {
        Command subCommand = readCommand(is);
        if (not subCommand)
        {
            throwError("'group " + std::to_string(howMany) + "' needs command #"
                       + std::to_string(i + 1) + "!");
        }
        subCommands.push_back(subCommand);
    }
    return [subCommands](Parameters parameters)
    {
        for (auto&& subCommand: subCommands)
        {
            subCommand(parameters);
        }
    };
}

TestCommands::Command TestCommands::readThreadCommand(std::istream &is)
{
    Command subCommand = readCommand(is);
    if (not subCommand)
    {
        throwError("'thread' needs sub command!");
    }
    return [subCommand](Parameters parameters)
    {
        std::thread t([subCommand, parameters] {
            subCommand(parameters);
        });
        t.detach();
    };
}

TestCommands::Command TestCommands::readWriteCommand(std::istream &is)
{
    std::string message = readArg<std::string>(is, "`write` needs message to write!");
    return [message](Parameters parameters)
    {
        parameters.printText(message);
    };
}

TestCommands::Command TestCommands::readWaitCommand(std::istream &is)
{
    std::uint32_t waitTime = readArg<std::uint32_t>(is, "'wait' needs wait time (ms)");
    return [waitTime](Parameters)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
    };
}

TestCommands::Command TestCommands::readSendCommand(std::istream &is)
{
    MessageId messageId = readArg<MessageId>(is, "'send' needs MessageId", MessageId::Sib);
    PhoneNumber from = readArg<PhoneNumber>(is, "'send' needs From(PhoneNumber)");
    PhoneNumber to = readArg<PhoneNumber>(is, "'send' needs From(PhoneNumber)");
    std::string messageBody = readMessageBody(is);
    OutgoingMessage messageBuilder(messageId, from, to);
    if (not messageBody.empty())
    {
        messageBuilder.writeText(messageBody);
    }
    auto message = messageBuilder.getMessage();

    return [to, message](Parameters parameters)
    {
        parameters.sendMessage(message, to);
    };
}

template <typename T>
T TestCommands::readArg(std::istream& is, std::string onFailure, T defaultValue)
{
    T value = defaultValue;
    if (not (is >> value))
    {
        throwError(onFailure);
    }
    return value;
}

std::string TestCommands::readMessageBody(std::istream &is)
{
    std::string body = readArg<std::string>(is, "Message body missing (for 0-bytes use 0x)");
    const std::string hexPrefix = "0x";
    if (body.substr(0, hexPrefix.length()) != hexPrefix)
    {
        return body;
    }

    // translate from hex (0x00000001 is {0,0,0,1} string}
    body = body.substr(hexPrefix.length());
    if (body.length() % 2 != 0)
    {
        throwError("This hex-string shall have even number of digits: " + body);
    }
    std::string hexBody;
    hexBody.reserve(body.length() / 2);
    for (std::string::size_type i = 0; i < body.length(); i += 2)
    {
        std::istringstream oneNumberStream(body.substr(i, 2));
        unsigned oneNumber;
        if (not (oneNumberStream >> std::hex >> oneNumber))
        {
            throwError(body.substr(i, 2) + ": is not hex number!");
        }
        hexBody += static_cast<char>(oneNumber);
    }
    return hexBody;
}

void TestCommands::throwError(std::string msg)
{
    throw std::runtime_error("Parse error: " + msg);
}

}
