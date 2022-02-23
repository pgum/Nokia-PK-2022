#pragma once

#include "IUeGui.hpp"

#include <QObject>
#include <QTextEdit>
#include <QKeyEvent>

#include "Logger/ILogger.hpp"

namespace ue
{

class QtSubmitTextEdit : public QTextEdit
{
    Q_OBJECT
signals:
    void submitted();
public:
    void keyPressEvent(QKeyEvent *event) override;
};

}

