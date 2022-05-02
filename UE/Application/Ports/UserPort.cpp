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
        SMS_DB ourDataBase = SMS_DB();
        SMS sms1 = SMS();
        SMS sms2 = SMS();
        ourDataBase.addSmsToDB(sms1);
        ourDataBase.addSmsToDB(sms2);
        for (auto i: ourDataBase.getAllSMS()) {
            menu.addSelectionListItem(i.getMTextMessage(), "");
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

}
