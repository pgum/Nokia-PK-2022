#pragma once

#include "IComponent.hpp"
#include "IApplicationEnvironment.hpp"
#include <memory>

namespace bts
{

std::unique_ptr<IComponent> createApplication(IApplicationEnvironment& environment);

}
