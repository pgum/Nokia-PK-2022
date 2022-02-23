#pragma once

#include <QObject>
#include <QByteArray>
#include "ITransport.hpp"
#include "Logger/ILogger.hpp"

class QAbstractSocket;

namespace bts
{

class QtTransport : public QObject, public ITransport
{
    Q_OBJECT;
public:
    QtTransport(common::ILogger& logger, QAbstractSocket* socket);
    ~QtTransport();

    void registerMessageCallback(MessageCallback messageCallback) override;
    void registerDisconnectedCallback(DisconnectedCallback disconnectedCallback) override;
    bool sendMessage(BinaryMessage message) override;

    std::string addressToString() const override;
private:
    void readMessageFromSocket();
    void handleClosingConnection();

    common::ILogger& logger;
    QAbstractSocket* socket;

    MessageCallback messageCallback;
    DisconnectedCallback disconnectedCallback;

private slots:
    bool sendMessageSlot(QByteArray message);

signals:
    bool sendMessageSignal(QByteArray message);

};

}
