#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>

#include "Ports/UserPort.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Mocks/IUeGuiMock.hpp"
#include "SMSDB/SMS.hpp"

namespace ue
{
using namespace ::testing;

class UserPortTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{112};
    std::vector<std::pair<unsigned int,std::shared_ptr< ITextMessage>>> smsContainer;

    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IUserEventsHandlerMock> handlerMock;
    StrictMock<IUeGuiMock> guiMock;
    StrictMock<IListViewModeMock> listViewModeMock;
    StrictMock<ITextModeMock> textModeMock;
    StrictMock<ISmsComposeModeMock> smsComposeModeMock;

    UserPort objectUnderTest{loggerMock, guiMock, PHONE_NUMBER};

    UserPortTestSuite()
    {
        smsContainer.emplace_back(0,std::make_unique<SMS>());
        smsContainer.emplace_back(1,std::make_unique<SMS>());
        smsContainer.emplace_back(2,std::make_unique<SMS>());

        EXPECT_CALL(guiMock, setTitle(HasSubstr(to_string(PHONE_NUMBER))));
        objectUnderTest.start(handlerMock);
    }
    ~UserPortTestSuite()
    {
        objectUnderTest.stop();
    }
};

TEST_F(UserPortTestSuite, shallStartStop)
{
}

TEST_F(UserPortTestSuite, shallShowNotConnected)
{
    EXPECT_CALL(guiMock, showNotConnected());
    objectUnderTest.showNotConnected();
}

TEST_F(UserPortTestSuite, shallShowConnecting)
{
    EXPECT_CALL(guiMock, showConnecting());
    objectUnderTest.showConnecting();
}

TEST_F(UserPortTestSuite, shallShowMainMenu)
{
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(1));
    objectUnderTest.showMainMenu();
}

TEST_F(UserPortTestSuite, shallShowSMSList)
{
    EXPECT_CALL(guiMock, showNewSms(false));
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(Exactly(3));
    objectUnderTest.showSMSList(smsContainer);
}

TEST_F(UserPortTestSuite, shallShowSingleSMSOnUserSelect)
{
    EXPECT_CALL(guiMock, showNewSms(false));
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(Exactly(3));

    objectUnderTest.showSMSList(smsContainer);

    EXPECT_CALL(guiMock,setViewTextMode()).WillOnce(ReturnRef(textModeMock));
    EXPECT_CALL(textModeMock, setText(_));

    objectUnderTest.showSMS(*smsContainer.at(0).second);
}

TEST_F(UserPortTestSuite, shallShowSMSCompose)
{
    EXPECT_CALL(guiMock,setSmsComposeMode()).WillOnce(ReturnRef(smsComposeModeMock));

    objectUnderTest.initSmsComposer();
}

}
