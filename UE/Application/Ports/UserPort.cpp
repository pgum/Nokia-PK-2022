#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ITextMode.hpp"
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
            menu.addSelectionListItem(i.getSmsHeader(), "");
        }
        gui.setAcceptCallback([this, &menu] { acceptCallbackClicked(menu); });
    }

    SMS UserPort::returnSingleSms(int smsListPosition) {
        return smsDb.getSingleSmsAt(smsListPosition);
    }

    void UserPort::showSingleSms(int smsListPosition) {
        IUeGui::ITextMode &menu = gui.setViewTextMode();
        menu.setText(returnSingleSms(smsListPosition - 1).getMTextMessage());
    }

    void UserPort::showConnected(int notification) {
        IUeGui::IListViewMode &menu = gui.setListViewMode();
        menu.clearSelectionList();
        menu.addSelectionListItem("Compose SMS", "");
        if (notification == 1) {
            menu.addSelectionListItem("[NEW] View SMS", "");
        } else if (notification == 0) {
            menu.addSelectionListItem("View SMS", "");
        }
        menu.addSelectionListItem("Make Call", "");
        gui.setAcceptCallback([this, &menu] { acceptCallbackClicked(menu); });
    }

    void UserPort::acceptCallbackClicked(IUeGui::IListViewMode &menu) {
        auto index = menu.getCurrentItemIndex();
        menuIndex = index.first ? index.second : -1;
        currentCallbackState();
    }

    IUeGui::ISmsComposeMode &UserPort::composeSmsMode() {
        return gui.setSmsComposeMode();
    }

    IUeGui::IDialMode &UserPort::dialComposeMode() {
        return gui.setDialMode();
    }

    int UserPort::getMenuIndex() {
        return menuIndex;
    }

    IUeGui::ITextMode &UserPort::callFromMode(const common::PhoneNumber callerNumber) {
        gui.setAlertMode().setText("Call from " + common::to_string(callerNumber));
        return gui.setAlertMode();
    }

    IUeGui::ITextMode &UserPort::callToMode(const common::PhoneNumber receiverNumber) {
        gui.setAlertMode().setText("Calling to " + common::to_string(receiverNumber));
        return gui.setAlertMode();
    }

    void UserPort::acceptCallback(IUeGui::Callback acceptCallback) {
        this->currentCallbackState = acceptCallback;
        gui.setAcceptCallback(acceptCallback);
    }

    void UserPort::rejectCallback(IUeGui::Callback rejectCallback) {
        gui.setRejectCallback(rejectCallback);
    }

    SMS_DB &UserPort::getSmsDB() {
        return smsDb;
    }

    void UserPort::setSmsDB(SMS_DB &smsDb) {
        UserPort::smsDb = smsDb;
    }

}
