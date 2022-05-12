#include "IUserPortMock.hpp"

namespace ue
{
IUserEventsHandlerMock::IUserEventsHandlerMock() = default;
IUserEventsHandlerMock::~IUserEventsHandlerMock() = default;

IUserPortMock::IUserPortMock(IUeGuiMock &userGuiMock) : gui(userGuiMock) {}

IUserPortMock::~IUserPortMock() = default;

IUeGui& IUserPortMock::getUserGui (){
    return gui;
}

}
