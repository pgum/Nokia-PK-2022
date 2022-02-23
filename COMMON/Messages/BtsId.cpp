#include "BtsId.hpp"

namespace common
{

std::istream& operator >> (std::istream& is, BtsId& obj)
{
    return is >> obj.value;
}

std::ostream& operator << (std::ostream& os, const BtsId& obj)
{
    return os << obj.value;
}

std::string to_string(const BtsId &obj)
{
    return std::to_string(obj.value);
}

}
