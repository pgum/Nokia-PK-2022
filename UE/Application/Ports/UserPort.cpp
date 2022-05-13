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

IUeGui& UserPort::getUserGui()
{
    return gui;
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
    smsView.setText(sms.getMessage());
}

void UserPort::showSMS(ITextMessage &&sms)
{
    IUeGui::ITextMode& smsView = gui.setViewTextMode();
    smsView.setText(sms.getMessage());
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
