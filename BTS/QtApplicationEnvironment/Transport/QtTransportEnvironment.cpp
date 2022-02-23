#include "QtTransportEnvironment.hpp"
#include "QtTransport.hpp"
#include <QTcpSocket>
#include <QtNetwork>
#include <QByteArray>

namespace bts
{

QtTransportEnvironment::QtTransportEnvironment(common::ILogger& logger, common::MultiLineConfig &config)
    : logger(logger),
      port(config.getNumber<decltype(port)>("port", 8181))
{}

QtTransportEnvironment::~QtTransportEnvironment()
{
    if (session)
        QObject::disconnect(session.get(), &QNetworkSession::opened, 0, 0);
    if (server)
        server->close();
}

void QtTransportEnvironment::exec()
{
    QNetworkConfigurationManager manager{};
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired)
    {
        logger.logDebug("Session needed");
        session.reset(new QNetworkSession(manager.defaultConfiguration()));
        QObject::connect(session.get(), &QNetworkSession::opened, std::bind(&QtTransportEnvironment::sessionOpened, this));
        session->open();
    }
    else
    {
        sessionOpened();
    }
    QObject::connect(server.get(), &QTcpServer::newConnection, std::bind(&QtTransportEnvironment::handleNewConnection, this));
}

void QtTransportEnvironment::sessionOpened()
{
    logger.logDebug("Session opened");
    server.reset(new QTcpServer());
    server->listen(QHostAddress::Any, port)
            ? logger.logInfo("server started, port: ", port)
            : logger.logError("server could not start, port: ", port);
}

void bts::QtTransportEnvironment::registerUeConnectedCallback(UeConnectedCallback ueConnectedCallback)
{
    this->ueConnectedCallback = ueConnectedCallback;
}

std::string QtTransportEnvironment::getAddress() const
{
    std::string result;
    std::string port = std::to_string(this->port);
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    for(auto&& host: list)
    {
        if (host.isLoopback())
        {
            continue;
        }
        if (host.protocol() == QAbstractSocket::IPv4Protocol)
        {
            result += "\n" + host.toString().toStdString() + ":" + port;
        }
    }
    return result;
}

void QtTransportEnvironment::handleNewConnection()
{
    QAbstractSocket* socket = server->nextPendingConnection();
    if (socket)
    {
        auto ueTransport = std::make_shared<QtTransport>(logger, socket);
        logger.logDebug("New connection from: ", ueTransport->addressToString());
        if (ueConnectedCallback)
        {
            ueConnectedCallback(ueTransport);
        }
        else
        {
            logger.logError("New connection from: ", ueTransport->addressToString(), " discarded, application not interested!");
        }
    }
    else
    {
        logger.logError("No new socked for new connection!");
    }
}

}
