#pragma once

#include <iostream>
#include <gmock/gmock.h>
#include "UeConnection/UeSlot.hpp"

namespace bts
{
void PrintTo(const UeSlot& usSlot, std::ostream* os);
}
