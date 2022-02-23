#pragma once

#include "IComponent.hpp"
#include "IApplicationEnvironment.hpp"
#include "Logger/PrefixedLogger.hpp"
#include <memory>
#include <initializer_list>
#include <vector>

namespace bts
{

class Application : public IComponent
{
public:
    Application(ILogger &logger, std::initializer_list<std::shared_ptr<IComponent>> components);
    ~Application();

    void start() override;
    void stop() override;

private:
    common::PrefixedLogger logger;
    std::vector<std::shared_ptr<IComponent>> components;
};

}
