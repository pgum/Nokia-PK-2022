#pragma once

#include "Messages/BtsId.hpp"
#include "IConsole.hpp"
#include "ITransport.hpp"
#include "Logger/Logger.hpp"

namespace bts
{

using common::ILogger;
using common::BtsId;

class IApplicationEnvironment
{
public:
    virtual ~IApplicationEnvironment() {}

    virtual IConsole& getConsole() = 0;
    virtual void registerUeConnectedCallback(UeConnectedCallback) = 0;
    virtual ILogger& getLogger() = 0;
    virtual BtsId getBtsId() const = 0;
    virtual std::string getAddress() const = 0;

    virtual void startMessageLoop() = 0;
};

}
