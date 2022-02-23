#pragma once

#include "IApplicationEnvironment.hpp"
#include <QCoreApplication>
#include "Console/TextConsole.hpp"
#include "Logger/Logger.hpp"
#include "Config/MultiLineConfig.hpp"
#include "Transport/QtTransportEnvironment.hpp"
#include <fstream>

namespace bts
{

class ApplicationEnvironment : public IApplicationEnvironment
{
public:
    ApplicationEnvironment(int& argc, char* argv[]);
    IConsole& getConsole() override;
    void registerUeConnectedCallback(UeConnectedCallback) override;
    ILogger& getLogger() override;
    BtsId getBtsId() const override;
    std::string getAddress() const override;


    void startMessageLoop() override;

private:
    std::unique_ptr<common::MultiLineConfig> configuration;
    BtsId btsId;
    std::ofstream logFile;
    common::Logger logger;

    QCoreApplication qApplication;
    TextConsole console;
    QtTransportEnvironment transportEnvironment;

    static std::unique_ptr<common::MultiLineConfig> readConfiguration(int argc, char* argv[]);
    static BtsId generateBtsId();
    static std::string logFilename(BtsId btsId);
};

}
