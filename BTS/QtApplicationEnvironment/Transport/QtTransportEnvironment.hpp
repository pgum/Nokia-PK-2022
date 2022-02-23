#pragma once
#include "ITransport.hpp"
#include "Logger/ILogger.hpp"
#include "Config/MultiLineConfig.hpp"

class QTcpServer;
class QNetworkSession;
class QAbstractSocket;

namespace bts
{

class QtTransportEnvironment
{
public:
    QtTransportEnvironment(common::ILogger& logger, common::MultiLineConfig& config);
    ~QtTransportEnvironment();

    void exec();
    void registerUeConnectedCallback(UeConnectedCallback ueConnectedCallback);
    std::string getAddress() const;

private:
    void sessionOpened();
    void handleNewConnection();

    common::ILogger& logger;
    std::uint32_t port;
    std::unique_ptr<QTcpServer> server;
    std::unique_ptr<QNetworkSession> session;
    UeConnectedCallback ueConnectedCallback;
};

}
