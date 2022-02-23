#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include "QtPhoneNumberEdit.hpp"
#include "QtStackedWidget.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Logger/ILogger.hpp"

namespace ue
{

class QtUeModeWidget : public QWidget
{
    Q_OBJECT
public:
    QtUeModeWidget(QtPhoneNumberEdit& phoneNumberEdit,
                   QtStackedWidget& stackedWidget);

    void init();
    void activate();

protected:
    void addChildWidget(QWidget* childWidget);

    void activateWithPhoneNumberEditEnabled();
    void activateWithPhoneNumberEditDisabled();

    PhoneNumber getPhoneNumber() const;

private:
    void activateThisWidget();

    QVBoxLayout mainLayout;

    QtPhoneNumberEdit& phoneNumberEdit;
    QtStackedWidget& stackedWidget;
    int stackedWidgetIndex;

signals:
    void activateSignal();

protected slots:
    virtual void activateSlot() = 0;
};

}

