#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ITextMode.hpp"
#include "UeGui/ISmsComposeMode.hpp"
#include "Models/Sms.hpp"

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

    void UserPort::showConnected() {
        IUeGui::IListViewMode &menu = gui.setListViewMode();
        menu.clearSelectionList();
        menu.addSelectionListItem("Compose SMS", "");
        if (smsDb.checkForNewSms()) {
            menu.addSelectionListItem("[!]View SMS", "");
            gui.showNewSms(false);
        } else {
            menu.addSelectionListItem("View SMS", "");
        }
        gui.setAcceptCallback([this, &menu] { onAcceptCallback(menu); });
    }

    std::optional <Sms> UserPort::viewSms(int index) {
        return smsDb.retrieveSms(index);
    }

    void UserPort::showSms(int index) {
        IUeGui::ITextMode &text = gui.setViewTextMode();
        std::optional <Sms> sms = viewSms(index);
        if (sms.has_value()) {
            text.setText(sms.value().getText());
        }
    }

    std::vector <Sms> UserPort::viewSmsList() {
        return smsDb.getSmsList();
    }

    void UserPort::showSmsList() {
        IUeGui::IListViewMode &menu = gui.setListViewMode();
        menu.clearSelectionList();

        for (auto &&sms: smsDb.getSmsList()) {
            std::string info = "FROM:" + common::to_string(sms.getFrom()) + ";TO:" + common::to_string(sms.getTo());
            if (!sms.isReceived()) {
                menu.addSelectionListItem("[FAILED]" + info, "");
            } else {
                menu.addSelectionListItem(info, "");
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
        if (!pair.first) {
            action = -1;
        } else {
            action = pair.second;
        }
        callback();
    }

    IUeGui::ISmsComposeMode &UserPort::composeSms() {
        IUeGui::ISmsComposeMode &mode = gui.setSmsComposeMode();
        mode.clearSmsText();
        return mode;
    }

    void UserPort::showCallRequest(common::PhoneNumber senderPhoneNumber) {
        auto &alertMode = gui.setAlertMode();
        alertMode.setText("Call from: " + to_string(senderPhoneNumber));
        auto accept = [&, senderPhoneNumber]() {
            handler->USER_handleCallAccept(senderPhoneNumber);
        };
        auto reject = [&, senderPhoneNumber]() {
            handler->USER_handleCallDrop(senderPhoneNumber);
        };
        gui.setAcceptCallback(accept);
        gui.setRejectCallback(reject);

    }

    void UserPort::callAchieved(common::PhoneNumber senderPhoneNumber) {
        logger.logDebug("Talking mode with: ", senderPhoneNumber);
        IUeGui::ICallMode &callMode = gui.setCallMode();
        callMode.appendIncomingText("Call from: " + to_string(senderPhoneNumber));

    }

    void UserPort::startTalking(common::PhoneNumber) {
        auto &view = gui.setCallMode();
        view.appendIncomingText("");
    }


    void UserPort::showPartnerNotAvailable(common::PhoneNumber receiverPhoneNumber) {
        gui.showPeerUserNotAvailable(receiverPhoneNumber);
    }

    void UserPort::showEnterPhoneNumber() {
        ue::IUeGui::IDialMode &dialModeMenu = gui.setDialMode();
        gui.setAcceptCallback([&]() {
            handler->handleCallRequest(dialModeMenu.getPhoneNumber());
        });
    }

    void UserPort::showDialing(common::PhoneNumber senderPhoneNumber) {
        logger.logDebug("Trying to connect with: ", senderPhoneNumber);
        IUeGui::ITextMode &dialModeMenu = gui.setAlertMode();
        dialModeMenu.setText("Trying to\nconnect with:\n" + to_string(senderPhoneNumber));
        gui.setAcceptCallback([&]() {});
        gui.setRejectCallback([&]() {
            handler->handleCallDrop(senderPhoneNumber);
        });
    }

}
