#include "Application.hpp"

namespace bts
{

Application::Application(ILogger& logger, std::initializer_list<std::shared_ptr<IComponent>> components)
    :   logger(logger, "[Application]"),
        components(components)
{}

Application::~Application()
{}

void Application::start()
{
    logger.logDebug("started");
    for (auto it = components.begin(); it != components.end(); ++it)
    {
        (*it)->start();
    }
}

void Application::stop()
{
    for (auto it = components.rbegin(); it != components.rend(); ++it)
    {
        (*it)->stop();
    }
    logger.logDebug("finished");

}

}
