#pragma once

#include "IApplicationEnvironment.hpp"
#include "GUI/QtApplication.hpp"
#include "Transport/Transport.hpp"
#include <QApplication>
#include "Logger/Logger.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Config/MultiLineConfig.hpp"
#include <fstream>

namespace ue
{

class ApplicationEnvironment : public IApplicationEnvironment
{
public:
    ApplicationEnvironment(int &argc, char* argv[]);
    IUeGui& getUeGui() override;
    ITransport& getTransportToBts() override;
    ILogger& getLogger() override;
    PhoneNumber getMyPhoneNumber() const override;
    std::int32_t getProperty(std::string const& name, std::int32_t defaultValue) const override;

    void startMessageLoop() override;

private:
    std::unique_ptr<common::MultiLineConfig> configuration;
    PhoneNumber myPhoneNumber;
    std::ofstream logFile;
    common::Logger loggerBase;
    common::PrefixedLogger logger;

    QApplication qApplication;
    QtUeGui gui;
    Transport transport;

    static std::unique_ptr<common::MultiLineConfig> readConfiguration(int argc, char* argv[]);


};

}
