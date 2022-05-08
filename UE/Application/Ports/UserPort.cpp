#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"
#include "SMS/SMS.hpp"
#include "SMS/SMS_DB.hpp"

namespace ue {

    UserPort::UserPort(common::ILogger &logger, IUeGui &gui, common::PhoneNumber phoneNumber)
            : logger(logger, "[USER-PORT]"),
              gui(gui),
              phoneNumber(phoneNumber) {}

    void UserPort::start(IUserEventsHandler &handler) {
        this->handler = &handler;
        gui.setTitle("Nokia " + to_string(phoneNumber));
    }

    void UserPort::stop() {
        handler = nullptr;
    }

    void UserPort::showNotConnected() {
        gui.showNotConnected();
    }

    void UserPort::showConnecting() {
        gui.showConnecting();
    }

    void UserPort::showViewingSms() {
        IUeGui::IListViewMode &menu = gui.setListViewMode();
        menu.clearSelectionList();
        for (auto i: smsDb.getAllSMS()) {
            menu.addSelectionListItem(i.getSmsHeader(),"");
        }
        gui.setAcceptCallback([this, &menu] { acceptCallbackClicked(menu); });
    }

    void UserPort::showConnected() {
        IUeGui::IListViewMode &menu = gui.setListViewMode();
        menu.clearSelectionList();
        menu.addSelectionListItem("Compose SMS", "");
        menu.addSelectionListItem("View SMS", "");
        gui.setAcceptCallback([this, &menu] { acceptCallbackClicked(menu); });
    }

    void UserPort::acceptCallbackClicked(IUeGui::IListViewMode &menu) {
        auto index = menu.getCurrentItemIndex();
        if (index.first) {
            menuIndex = 1;
        }
        if (index.second) {
            menuIndex = 2;
        }
        currentCallbackState();
    }

    IUeGui::ISmsComposeMode& UserPort::composeSmsMode() {
        return gui.setSmsComposeMode();
    }

    int UserPort::getMenuIndex() {
        return menuIndex;
    }

    void UserPort::acceptCallback(IUeGui::Callback acceptCallback) {
        this->currentCallbackState = acceptCallback;
        gui.setAcceptCallback(acceptCallback);
    }

    void UserPort::rejectCallback(IUeGui::Callback rejectCallback) {
        gui.setRejectCallback(rejectCallback);
    }

    SMS_DB & UserPort::getSmsDB(){
        return smsDb;
    }

    void UserPort::setSmsDB(SMS_DB &smsDb) {
        UserPort::smsDb = smsDb;
    }

}
