#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ConsoleCommands.hpp"

#include "Mocks/IConsoleMock.hpp"
#include "Mocks/IApplicationEnvironmentMock.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IUeRelayMock.hpp"
#include "Mocks/IUeConnectionMock.hpp"

namespace bts
{

class ConsoleCommandsTestSuite : public ::testing::Test
{
protected:
    ConsoleCommandsTestSuite();

    static void expectRegisterCallback(IConsoleMock&, std::string command, IConsole::CommandCallback &listAttachedUeCallback);
    void expectRegisterCallbacks();

    SyncGuardPtr syncGuard;
    testing::StrictMock<IConsoleMock> consoleMock;
    testing::StrictMock<IApplicationEnvironmentMock> environmentMock;
    testing::NiceMock<common::ILoggerMock> loggerMock;
    std::shared_ptr<IUeRelayMock> ueRelayMock;
    std::unique_ptr<ConsoleCommands> objectUnderTest;

    IConsole::CommandCallback showAddressCallback;
    IConsole::CommandCallback showStatusCallback;
    IConsole::CommandCallback listAttachedUeCallback;
    IConsole::CommandCallback testCommandsCallback;
};

class ConsoleCommandsAfterStartTestSuite : public ConsoleCommandsTestSuite
{
protected:
    ConsoleCommandsAfterStartTestSuite();

    void onCallback(IConsole::CommandCallback &callback, std::string args = "");
    void applyUeAttached(IUeRelay::UeVisitor visitor);
    void expectCountAttached();
    void expectCountNotAttached();
    void expectAttachedPrinted();
    void expectGetBtsId();
    void expectGetAddress();
    void assertResultContainsCountAttached();
    void assertResultContainsCountNotAttached();
    void assertResultContainsAttachedPrintouts();
    void assertResultContainsBtsId();
    void assertResultContainsAddress();

    const std::size_t COUNT_ATTACHED = 711;
    const std::size_t COUNT_NOT_ATTACHED = 1819;
    const BtsId BTS_ID{17};
    const std::string ADDRESS = "www.aaa.bbb:123";

    static constexpr std::size_t COUNT_ATTACHED_TO_VISIT = 3;
    ::testing::StrictMock<IUeConnectionMock> ueConnectionAttachedMock[COUNT_ATTACHED_TO_VISIT];
    std::string ueConnectionAttachedPrintout[COUNT_ATTACHED_TO_VISIT] = {
      "FirstAttached",
      "SecondAttached",
      "ThirdAttached"
    };
    std::string result;

};


}
