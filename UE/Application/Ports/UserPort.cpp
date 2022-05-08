#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"

namespace ue
{

UserPort::UserPort(common::ILogger &logger, IUeGui &gui, common::PhoneNumber phoneNumber)
    : logger(logger, "[USER-PORT]"),
      gui(gui),
      phoneNumber(phoneNumber)
{}

void UserPort::start(IUserEventsHandler &handler)
{
    this->handler = &handler;
    gui.setTitle("Nokia " + to_string(phoneNumber));
}

void UserPort::stop()
{
    handler = nullptr;
}

void UserPort::showNotConnected()
{
    gui.showNotConnected();
}

void UserPort::showConnecting()
{
    gui.showConnecting();
}

void UserPort::showConnected(ISMSDatabase& smsDb)
{
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();
    menu.addSelectionListItem("Compose SMS", "");
    menu.addSelectionListItem("View SMS", "");

    gui.setAcceptCallback([this,&menu,&smsDb]() -> void
    {
        this->handleMainMenuInput(smsDb);
    });

    gui.setRejectCallback([this,&smsDb]{
        showConnected(smsDb);
    });
}

void UserPort::handleMainMenuInput(ISMSDatabase& smsDb)
{
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    auto elementSelected = menu.getCurrentItemIndex();

    gui.setRejectCallback([]{ return; });
    gui.setAcceptCallback([]{ return; });

    if(!elementSelected.first) // no selection
    {
        return;
    }

    switch (elementSelected.second)
    {
        case COMPOSE_SMS:
        {
            showNewSmsCompose(smsDb);
            break;
        }
        case VIEW_SMS_LIST:
        {
            showSMSList(smsDb);
            break;
        }
        default:
        {
            return;
        }

    }

}

void UserPort::showNewSmsCompose(ISMSDatabase& smsDb)
{
    IUeGui::ISmsComposeMode& smsCompose = gui.setSmsComposeMode();

    gui.setRejectCallback([&]{ showConnected(smsDb); });
    gui.setAcceptCallback([&]{ handleNewSMSInput(smsDb,smsCompose); });

}


void UserPort::handleNewSMSInput(ISMSDatabase &smsDb, IUeGui::ISmsComposeMode& smsCompose)
{
    auto text = smsCompose.getSmsText();
    auto number = smsCompose.getPhoneNumber();

    smsDb.addSMS(phoneNumber,number,text);
    smsCompose.clearSmsText();
    showConnected(smsDb);

}


void UserPort::showSMSList(ISMSDatabase& smsDb)
{

    gui.setRejectCallback([this,&smsDb]{ showConnected(smsDb); });
    gui.setAcceptCallback([this,&smsDb](){ showSelectedSMS(smsDb); });


    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();
    auto allSMS = smsDb.getAllSMS();

    std::for_each(allSMS.begin(), allSMS.end(), [&](auto& sms)
    {
        menu.addSelectionListItem(sms.second.getMessageSummary(),"");
    });


}


void UserPort::showSelectedSMS(ISMSDatabase &smsDb)
{
    gui.setRejectCallback([this,&smsDb] { showSMSList(smsDb); });


    IUeGui::IListViewMode& menu = gui.setListViewMode();
    auto elementSelected = menu.getCurrentItemIndex();

    if(!elementSelected.first)
    {
        showSMSList(smsDb);
        return;
    }

    SMS sms = smsDb.getAllSMS().at(elementSelected.second).second;

    menu.clearSelectionList();

    IUeGui::ITextMode& smsView = gui.setViewTextMode();
    smsView.setText(sms.getMessage());


}


}
