#include "UeSlotPrint.hpp"
#include "UeConnectionTestSuite.hpp"

namespace bts
{

void PrintTo(const UeSlot& ueSlot, ::std::ostream* os)
{
    *os << "attached: " << ueSlot.isAttached()
        << ", address: " << (void*)&ueSlot;
}

}
