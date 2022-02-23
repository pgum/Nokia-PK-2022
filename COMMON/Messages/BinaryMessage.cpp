#include "BinaryMessage.hpp"
#include <iomanip>
#include <sstream>
#include <string>

namespace common
{

constexpr const std::size_t BinaryMessage::MAX_SIZE;


std::ostream& operator << (std::ostream& os, const BinaryMessage& message)
{
    std::ios originalState(nullptr);
    originalState.copyfmt(os);

    for (auto&& b: message.value)
    {
        os << std::hex << std::setfill('0') << std::setw(2) << static_cast<std::uint32_t>(b);
    }

    os.copyfmt(originalState);
    return os;
}

std::istream& operator >> (std::istream& is, BinaryMessage& message)
{
    std::ios originalState(nullptr);
    originalState.copyfmt(is);

    std::string hexText;
    is >> hexText;
    if (hexText.length() % 2 != 0)
    {
        hexText = "0" + hexText;
    }
    message.value.clear();
    message.value.reserve(hexText.length() / 2);
    for (std::string::size_type i = 0; i < hexText.length(); i += 2)
    {
        std::istringstream oneNumberStream(hexText.substr(i, 2));
        unsigned oneNumber;
        if (not (oneNumberStream >> std::hex >> oneNumber))
        {
            is.setstate(std::ios_base::failbit);
            break;
        }
        message.value.push_back(static_cast<std::uint8_t>(oneNumber));
    }

    is.copyfmt(originalState);
    return is;
}

}
