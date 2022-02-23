#pragma once

#include "Messages/PhoneNumber.hpp"
#include "Messages/BinaryMessage.hpp"
#include <vector>
#include <map>
#include <functional>
#include <string>

namespace common
{

class TestCommands
{
public:
    TestCommands(std::string args);

    using PrintText = std::function<void(std::string)>;
    using SendMessage = std::function<void(const BinaryMessage&,
                                           PhoneNumber /*to*/)>;
    struct Parameters
    {
        PrintText printText;
        SendMessage sendMessage;
    };
    void run(Parameters parameters);

private:
    using Command = std::function<void(Parameters)>;
    using Commands = std::vector<Command>;
    Commands commands;

    Command readCommand(std::istream& is);
    Command readRepeatCommand(std::istream& is);
    Command readWaitCommand(std::istream& is);
    Command readSendCommand(std::istream& is);
    Command readGroupCommand(std::istream& is);
    Command readThreadCommand(std::istream& is);
    Command readWriteCommand(std::istream& is);

    template <typename T>
    T readArg(std::istream& is, std::string onFailure, T defaultValue = T{});
    std::string readMessageBody(std::istream& is);

    void throwError(std::string msg);

    using ReadCommandFunction = Command(TestCommands::*)(std::istream& is);
    using TextCommandMap = std::map<std::string, ReadCommandFunction>;
    static const TextCommandMap textCommandMap;
};

}
