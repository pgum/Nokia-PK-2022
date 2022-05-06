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

void UserPort::showConnected()
{
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();
    menu.addSelectionListItem("Compose SMS", "");
    menu.addSelectionListItem("View SMS", "");
    gui.setAcceptCallback([this, &menu] { onAcceptCallback(menu); });
}

void UserPort::viewSms(int index) {
    std::optional<Sms> sms = smsDb.retrieveSms(index);
    IUeGui::ITextMode&  text = gui.setViewTextMode();
    if(sms.has_value()){
        text.setText(sms.value().getText());
    }
}

void UserPort::showSms(int index) {
    viewSms(index);
}

void UserPort::viewSmsList() {
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();
    for(auto sms : smsDb.getSmsList()){
        menu.addSelectionListItem(sms.getText(), "");
    }
    gui.setAcceptCallback([this, &menu] { onAcceptCallback(menu); });
}

void UserPort::showSmsList() {
    viewSmsList();
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


}
