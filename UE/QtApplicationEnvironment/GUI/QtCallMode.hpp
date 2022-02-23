#pragma once

#include <QTextEdit>

#include "UeGui/ICallMode.hpp"
#include "QtSubmitTextEdit.hpp"
#include "QtUeModeWidget.hpp"


namespace ue
{

class QtCallMode : public QtUeModeWidget, public IUeGui::ICallMode
{
    Q_OBJECT
public:
    QtCallMode(QtPhoneNumberEdit& phoneNumberEdit,
               QtStackedWidget& stackedWidget);

    void activateForDialMode();

    void appendIncomingText(const std::string &text) override;
    void clearOutgoingText() override;
    std::string getOutgoingText() const override;

private:
    void constructGUI();
    void connectSignals();

    QTextEdit incomingTextEdit;
    QtSubmitTextEdit outgoingTextEdit;

signals:
    void activateForDialModeSignal();
    void appendTalkTextSignal(QString);
    void textEntered();

private slots:
    void activateSlot() override;
    void activateForDialModeSlot();
    void appendTalkTextSlot(QString text);
};

}

