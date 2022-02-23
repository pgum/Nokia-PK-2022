#pragma once

#include "IApplicationEnvironment.hpp"
#include <memory>

namespace bts
{

std::unique_ptr<IApplicationEnvironment> createApplicationEnvironment(int& argc, char* argv[]);

}
