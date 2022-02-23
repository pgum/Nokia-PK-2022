#include "ApplicationFactory.hpp"
#include "ApplicationEnvironmentFactory.hpp"

int main(int argc, char *argv[])
{
    using namespace bts;
    auto environment = createApplicationEnvironment(argc, argv);
    auto application = createApplication(*environment);
    application->start();
    environment->startMessageLoop();
    application->stop();
}
