#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ITextMode.hpp"

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

IUeGui::IDialMode& UserPort::initDialMode()
{
    return gui.setDialMode();
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
    menu.addSelectionListItem("Dial", "");
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

common::PhoneNumber UserPort::getInputPhoneNumber(IUeGui::IDialMode &dial)
{
    return  dial.getPhoneNumber();
}

std::string UserPort::getInputString(IUeGui::ISmsComposeMode& composer)
{
    return composer.getSmsText();
}


void UserPort::showNewCallRequest(common::PhoneNumber from) {
    IUeGui::ITextMode& incomingCall = gui.setAlertMode();
    incomingCall.setText("Incoming call from " + to_string(from));
}

void UserPort::showTalking()
{
    IUeGui::ICallMode& call = gui.setCallMode();

}

void UserPort::showDialing(common::PhoneNumber to)
{
    IUeGui::ITextMode& dialing = gui.setAlertMode();
    dialing.setText("Dialling to " + to_string(to) + "...");
}

void UserPort::showPartnerNotAvailable()
{
    IUeGui::ITextMode& info = gui.setAlertMode();
    info.setText("Number is not available");
}

void UserPort::showCallDropped()
{
    IUeGui::ITextMode& info = gui.setAlertMode();
    info.setText("Call rejected");
}

}
