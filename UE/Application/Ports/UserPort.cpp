#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ITextMode.hpp"
#include "UeGui/ISmsComposeMode.hpp"
#include "Models/Sms.hpp"

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

void UserPort::showConnected()
{
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();
    menu.addSelectionListItem("Compose SMS", "");
    if(smsDb.checkForNewSms())
    {
        menu.addSelectionListItem("[!]View SMS", "");
        gui.showNewSms(false);
    }
    else
    {
        menu.addSelectionListItem("View SMS", "");
    }
    gui.setAcceptCallback([this, &menu] { onAcceptCallback(menu); });
}

std::optional<Sms> UserPort::viewSms(int index) {
    return smsDb.retrieveSms(index);
}

void UserPort::showSms(int index) {
    IUeGui::ITextMode&  text = gui.setViewTextMode();
    std::optional<Sms> sms = viewSms(index);
    if(sms.has_value()){
        text.setText(sms.value().getText());
    }
}

std::vector<Sms> UserPort::viewSmsList() {
    return smsDb.getSmsList();
}

void UserPort::showSmsList() {
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();

    for(auto&& sms : smsDb.getSmsList()){
        std::string info = "FROM:" + common::to_string(sms.getFrom()) + ";TO:" + common::to_string(sms.getTo());
        if(!sms.isReceived())
        {
            menu.addSelectionListItem("[FAILED]" + info, "");
        }
        else
        {
            menu.addSelectionListItem(info,  "");
        }
    }
    gui.setAcceptCallback([this, &menu] { onAcceptCallback(menu); });
}

SmsDb &UserPort::getSmsDb() {
    return smsDb;
}

int UserPort::getAction() {
    return action;
}

void UserPort::acceptCallback(IUeGui::Callback acceptCallback) {
    this->callback = acceptCallback;
    gui.setAcceptCallback(acceptCallback);
}

void UserPort::rejectCallback(IUeGui::Callback rejectCallback) {
    gui.setRejectCallback(rejectCallback);
}

void UserPort::onAcceptCallback(IUeGui::IListViewMode &menu) {
    IUeGui::IListViewMode::OptionalSelection pair = menu.getCurrentItemIndex();
    if(!pair.first) {
        action = -1;
    }
    else {
        action = pair.second;
    }
    callback();
}

IUeGui::ISmsComposeMode& UserPort::composeSms() {
    IUeGui::ISmsComposeMode &mode = gui.setSmsComposeMode();
    mode.clearSmsText();
    return mode;
}

}
