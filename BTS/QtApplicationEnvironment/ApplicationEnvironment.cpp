#include <ApplicationEnvironment.hpp>
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <thread>
#include "Messages.hpp"

namespace bts
{

ApplicationEnvironment::ApplicationEnvironment(int& argc, char* argv[])
    : configuration(ApplicationEnvironment::readConfiguration(argc, argv)),
      btsId(BtsId{configuration->getNumber("id", generateBtsId().value)}),
      logFile(logFilename(btsId)),
      logger(logFile),
      qApplication(argc, argv),
      console(logger),
      transportEnvironment(logger, *configuration)
{
    QObject::connect(&console, SIGNAL(quit()), &qApplication, SLOT(quit()));
}

IConsole &ApplicationEnvironment::getConsole()
{
    return console;
}

void ApplicationEnvironment::registerUeConnectedCallback(UeConnectedCallback newCallback)
{
    transportEnvironment.registerUeConnectedCallback(newCallback);
}

ILogger &ApplicationEnvironment::getLogger()
{
    return logger;
}

BtsId ApplicationEnvironment::getBtsId() const
{
    return btsId;
}

std::string ApplicationEnvironment::getAddress() const
{
    return transportEnvironment.getAddress();
}

void ApplicationEnvironment::startMessageLoop()
{
    std::thread consoleThread([this] {
        logger.logDebug("Console loop started");
        console.run();
        logger.logDebug("Console loop finished");
    });
    logger.logDebug("Application loop started");
    transportEnvironment.exec();
    qApplication.exec();
    logger.logDebug("Application loop finished");
    consoleThread.join();
}

std::unique_ptr<common::MultiLineConfig> ApplicationEnvironment::readConfiguration(int argc, char *argv[])
{
    auto commandLineConfig = std::make_unique<common::MultiLineConfig>(argc - 1, argv + 1);

    std::string configFile = commandLineConfig->getString("config", "config");

    try
    {
        std::ifstream configStream;
        configStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        configStream.open(configFile);

        common::MultiLineConfig fileConfig(configStream);
        commandLineConfig->insertFrom(fileConfig);
    }
    catch (...)
    {
        std::clog << "Note: config file: \"" << configFile << "\" is not present or reading failure.\n\t((only command line arguments are used))" << std::endl;
    }
    return commandLineConfig;
}

BtsId ApplicationEnvironment::generateBtsId()
{
    std::srand(time(0));
    return BtsId{static_cast<decltype(BtsId::value)>(rand())};
}

std::string ApplicationEnvironment::logFilename(BtsId btsId)
{
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto localNow = localtime(&now);
    char timeBuff[20];
    strftime(timeBuff, sizeof(timeBuff), "%Y%m%d%H%M%S", localNow);

    std::ostringstream os;
    os << "bts" << btsId << "_syslog_" << timeBuff << ".txt";
    return os.str();
}

}
