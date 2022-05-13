#include "Application.hpp"
#include "ApplicationEnvironmentFactory.hpp"
#include "Ports/BtsPort.hpp"
#include "Ports/UserPort.hpp"
#include "Ports/TimerPort.hpp"
#include "SMSDB/BasicSMSDatabase.hpp"

int main(int argc, char* argv[])
{
    using namespace ue;
    using namespace std::chrono_literals;

    auto appEnv = ue::createApplicationEnvironment(argc, argv);
    auto& logger = appEnv->getLogger();
    auto& transport = appEnv->getTransportToBts();
    auto& gui = appEnv->getUeGui();
    auto phoneNumber = appEnv->getMyPhoneNumber();

    BtsPort bts(logger, transport, phoneNumber);
    UserPort user(logger, gui, phoneNumber);
    TimerPort timer(logger);

    BasicSMSDatabase smsDb{};
    smsDb.addSMS(PhoneNumber{123},phoneNumber,"1111111111111111111111111111111111111");
    smsDb.addSMS(PhoneNumber{123},phoneNumber,"2222222222222222222222222222222222222");
    Application app(phoneNumber, logger, bts, user, timer, smsDb);

    bts.start(app);
    user.start(app);
    timer.start(app);

    appEnv->startMessageLoop();

    bts.stop();
    user.stop();
    timer.stop();
}

