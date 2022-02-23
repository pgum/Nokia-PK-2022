#include "ConsoleCommandsTestSuite.hpp"
#include <sstream>

using namespace ::testing;

namespace bts
{


ConsoleCommandsTestSuite::ConsoleCommandsTestSuite()
{
    ueRelayMock = std::make_shared<StrictMock<IUeRelayMock>>();
    syncGuard = std::make_shared<SyncGuard>();
    objectUnderTest = std::make_unique<ConsoleCommands>(consoleMock, environmentMock, loggerMock, ueRelayMock, syncGuard);
}

void ConsoleCommandsTestSuite::expectRegisterCallback(IConsoleMock &consoleMock,
                                                      std::string command,
                                                      IConsole::CommandCallback& consoleCallback)
{
    EXPECT_CALL(consoleMock, addCommand(command, _, _)).WillOnce(SaveArg<2>(&consoleCallback));
}

void ConsoleCommandsTestSuite::expectRegisterCallbacks()
{
    expectRegisterCallback(consoleMock, "a", showAddressCallback);
    expectRegisterCallback(consoleMock, "s", showStatusCallback);
    expectRegisterCallback(consoleMock, "l", listAttachedUeCallback);
    EXPECT_CALL(consoleMock, addCloseCommand(_, _, _));
    EXPECT_CALL(consoleMock, addHelpCommand(_, _));
    expectRegisterCallback(consoleMock, "t", testCommandsCallback);
}

TEST_F(ConsoleCommandsTestSuite, shallRegisterCommandsOnStart)
{
    expectRegisterCallbacks();
    objectUnderTest->start();
}

ConsoleCommandsAfterStartTestSuite::ConsoleCommandsAfterStartTestSuite()
{
    expectRegisterCallbacks();
    objectUnderTest->start();
    Mock::VerifyAndClearExpectations(&consoleMock);
}

void ConsoleCommandsAfterStartTestSuite::expectCountAttached()
{
    EXPECT_CALL(*ueRelayMock, countAttached()).WillOnce(Return(COUNT_ATTACHED));
}

void ConsoleCommandsAfterStartTestSuite::expectCountNotAttached()
{
    EXPECT_CALL(*ueRelayMock, countNotAttached()).WillOnce(Return(COUNT_NOT_ATTACHED));
}

void ConsoleCommandsAfterStartTestSuite::assertResultContainsCountNotAttached()
{
    ASSERT_THAT(result, HasSubstr(std::to_string(COUNT_NOT_ATTACHED)));
}

void ConsoleCommandsAfterStartTestSuite::assertResultContainsCountAttached()
{
    ASSERT_THAT(result, HasSubstr(std::to_string(COUNT_ATTACHED)));
}

void ConsoleCommandsAfterStartTestSuite::onCallback(IConsole::CommandCallback& callback, std::string args)
{
    std::ostringstream resultStream;
    callback(args, resultStream);
    result = resultStream.str();
}

void ConsoleCommandsAfterStartTestSuite::expectAttachedPrinted()
{
    for (std::size_t i = 0u; i < COUNT_ATTACHED_TO_VISIT; ++i)
    {
        EXPECT_CALL(ueConnectionAttachedMock[i], print(_)).WillOnce(Invoke([&,i](auto& os) { os << ueConnectionAttachedPrintout[i]; }));
    }
    EXPECT_CALL(*ueRelayMock, visitAttachedUe(_)).WillOnce(Invoke(this, &ConsoleCommandsAfterStartTestSuite::applyUeAttached));
}

void ConsoleCommandsAfterStartTestSuite::expectGetBtsId()
{
    EXPECT_CALL(environmentMock, getBtsId()).WillOnce(Return(BTS_ID));
}

void ConsoleCommandsAfterStartTestSuite::expectGetAddress()
{
    EXPECT_CALL(environmentMock, getAddress()).WillOnce(Return(ADDRESS));
}

void ConsoleCommandsAfterStartTestSuite::applyUeAttached(IUeRelay::UeVisitor visitor)
{
    for (auto& ue : ueConnectionAttachedMock)
    {
        visitor(ue);
    }
}

void ConsoleCommandsAfterStartTestSuite::assertResultContainsAttachedPrintouts()
{
    for (auto& uePrintout : ueConnectionAttachedPrintout)
    {
        ASSERT_THAT(result, HasSubstr(uePrintout));
    }
}

void ConsoleCommandsAfterStartTestSuite::assertResultContainsBtsId()
{
    ASSERT_THAT(result, HasSubstr(to_string(BTS_ID)));
}

void ConsoleCommandsAfterStartTestSuite::assertResultContainsAddress()
{
    ASSERT_THAT(result, HasSubstr(ADDRESS));
}

TEST_F(ConsoleCommandsAfterStartTestSuite, shallShowAddress)
{
    expectGetBtsId();
    expectGetAddress();

    onCallback(showAddressCallback);

    assertResultContainsBtsId();
    assertResultContainsAddress();
}

TEST_F(ConsoleCommandsAfterStartTestSuite, shallShowStatus)
{
    expectCountNotAttached();
    expectCountAttached();

    onCallback(showStatusCallback);

    assertResultContainsCountNotAttached();
    assertResultContainsCountAttached();
}

TEST_F(ConsoleCommandsAfterStartTestSuite, shallListAttached)
{
    expectAttachedPrinted();
    onCallback(listAttachedUeCallback);
    assertResultContainsAttachedPrintouts();
}

}
