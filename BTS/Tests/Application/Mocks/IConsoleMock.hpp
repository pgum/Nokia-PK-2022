#pragma once

#include <gmock/gmock.h>
#include "IConsole.hpp"

namespace bts
{

class IConsoleMock : public IConsole
{
public:
    IConsoleMock();
    ~IConsoleMock() override;

    MOCK_METHOD(void, addCommand, (std::string command, const std::string &commandText, CommandCallback), (final));
    MOCK_METHOD(void, addCloseCommand, (std::string command, const std::string &commandText, CommandCallback), (final));
    MOCK_METHOD(void, addHelpCommand, (std::string command, const std::string &commandText), (final));
};


}
