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

common::PhoneNumber UserPort::getPhoneNumber()
{
    return phoneNumber;
}

IUeGui::ISmsComposeMode& UserPort::initSmsComposer()
{
    return gui.setSmsComposeMode();
}

IUeGui::IListViewMode& UserPort::initListViewMode()
{
    return gui.setListViewMode();
}

IUeGui::ITextMode& UserPort::initTextMode()
{
    return gui.setViewTextMode();
}

void UserPort::setAcceptCallback(const IUeGui::Callback& callback)
{
    gui.setAcceptCallback(callback);
}

void UserPort::setRejectCallback(const IUeGui::Callback& callback)
{
    gui.setRejectCallback(callback);
}

void UserPort::setHomeCallback(const IUeGui::Callback& callback)
{
    throw std::logic_error("setHomeCallback not implemented");
}


void UserPort::showMainMenu()
{
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();
    menu.addSelectionListItem("Compose SMS", "");
    menu.addSelectionListItem("View SMS", "");
}




void UserPort::showSMSList(const smsContainer&& smsList)
{

    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();

    std::for_each(smsList.begin(), smsList.end(), [&menu](auto& sms)
    {
        menu.addSelectionListItem(sms.second->getMessageSummary(),"");
    });

}





void UserPort::showSMSList(const smsContainer& smsList)
{
    gui.showNewSms(false);
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();

    std::for_each(smsList.begin(), smsList.end(), [&menu](auto& sms)
    {
        menu.addSelectionListItem(sms.second->getMessageSummary(),"");
    });

}

void UserPort::showSMS(ITextMessage &sms)
{
    IUeGui::ITextMode& smsView = gui.setViewTextMode();
    std::string header;
    if(sms.getFromNumber()==phoneNumber)
    {
        header = "from You to " + common::to_string(sms.getToNumber()) + "\n\n";
    }
    else
    {
        header = "from " + common::to_string(sms.getFromNumber()) + "\n\n";
    }
    smsView.setText( header + sms.getMessage());
    sms.setIsReadStatus(true);
}

void UserPort::showSMS(ITextMessage &&sms)
{
    showSMS(sms);
}

void UserPort::showSMSNotification()
{
    gui.showNewSms(true);
}


common::PhoneNumber UserPort::getInputPhoneNumber(IUeGui::ISmsComposeMode& composer)
{
    return composer.getPhoneNumber();
}

std::string UserPort::getInputString(IUeGui::ISmsComposeMode& composer)
{
    return composer.getSmsText();
}

}
