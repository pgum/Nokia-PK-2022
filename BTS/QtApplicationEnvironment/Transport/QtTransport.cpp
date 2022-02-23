#include "QtTransport.hpp"
#include <QTcpSocket>
#include <QHostAddress>
#include "Messages/OutgoingMessage.hpp"
#include "Messages/IncomingMessage.hpp"

namespace bts
{

QtTransport::QtTransport(common::ILogger &logger, QAbstractSocket *socket)
    : logger(logger),
      socket(socket)
{
    QObject::connect(socket, &QAbstractSocket::readyRead, std::bind(&QtTransport::readMessageFromSocket, this));
    QObject::connect(socket, &QAbstractSocket::disconnected, std::bind(&QtTransport::handleClosingConnection, this));
    QObject::connect(this, SIGNAL(sendMessageSignal(QByteArray)), this, SLOT(sendMessageSlot(QByteArray)));
}

QtTransport::~QtTransport()
{
    QObject::disconnect(socket, &QAbstractSocket::readyRead, 0, 0);
    QObject::disconnect(socket, &QAbstractSocket::disconnected, 0, 0);
    logger.logDebug("QtTransport: bye");
}

void QtTransport::registerMessageCallback(ITransport::MessageCallback messageCallback)
{
    this->messageCallback = messageCallback;
}

void QtTransport::registerDisconnectedCallback(ITransport::DisconnectedCallback disconnectedCallback)
{
    this->disconnectedCallback = disconnectedCallback;
}

bool QtTransport::sendMessage(BinaryMessage message)
{
    common::OutgoingMessage sizeEncoder;
    sizeEncoder.writeNumber<BinaryMessage::SizeType>(message.value.size());
    BinaryMessage size = sizeEncoder.getMessage();

    QByteArray array{};
    array.append(reinterpret_cast<char*>(size.value.data()), size.value.size());
    array.append(reinterpret_cast<char*>(message.value.data()), message.value.size());
    return emit sendMessageSignal(std::move(array));
}

bool QtTransport::sendMessageSlot(QByteArray message)
{
    logger.logDebug("Send message to: ", addressToString());
    socket->write(std::move(message));
    socket->flush();
    return true;
}

std::string QtTransport::addressToString() const
{
    return socket->peerAddress().toString().toStdString() + "-" + std::to_string(socket->peerPort());
}

void QtTransport::handleClosingConnection()
{
    if (disconnectedCallback)
    {
        logger.logDebug("Connection lost from: ", addressToString());
        disconnectedCallback();
    }
    else
    {
        logger.logError("Connection lost from: ", addressToString(), " - application not interested!");
    }
}

void QtTransport::readMessageFromSocket()
{
    const std::size_t sizeSize = sizeof(BinaryMessage::SizeType);
    quint64 bytesAvailable;
    while ((bytesAvailable = socket->bytesAvailable()) >= sizeSize)
    {
        BinaryMessage sizeEncoded{ BinaryMessage::Value(sizeSize) };
        socket->read(reinterpret_cast<char*>(sizeEncoded.value.data()), sizeSize);
        common::IncomingMessage sizeDecoder(sizeEncoded);
        BinaryMessage::SizeType messageLength = sizeDecoder.readNumber<BinaryMessage::SizeType>();

        if (bytesAvailable < sizeSize + messageLength)
        {
            logger.logError("Wrong size: ", std::size_t(messageLength),
                            " - available bytes: ", bytesAvailable);
            continue;
        }

        BinaryMessage message{ BinaryMessage::Value(messageLength) };
        socket->read(reinterpret_cast<char*>(message.value.data()), messageLength);
        logger.logDebug("Message received from: ", addressToString(), " body: ", message);

        if (messageCallback)
        {
            messageCallback(std::move(message));
        }
        else
        {
            logger.logError("Message received from: ", addressToString(), " - application not interested");
        }
    }
}

}
