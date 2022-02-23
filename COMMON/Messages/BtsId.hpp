#pragma once

#include <cstdint>
#include <iostream>
#include <string>

namespace common
{

struct BtsId
{
    std::uint32_t value;
};

inline bool operator == (const BtsId& lhs, const BtsId& rhs)
{
    return lhs.value == rhs.value;
}
inline bool operator != (const BtsId& lhs, const BtsId& rhs)
{
    return !(lhs == rhs);
}

inline bool operator < (const BtsId& lhs, const BtsId& rhs)
{
    return lhs.value < rhs.value;
}
inline bool operator <= (const BtsId& lhs, const BtsId& rhs)
{
    return !(rhs < lhs);
}
inline bool operator >= (const BtsId& lhs, const BtsId& rhs)
{
    return !(lhs < rhs);
}
inline bool operator > (const BtsId& lhs, const BtsId& rhs)
{
    return (rhs < lhs);
}

std::istream& operator >> (std::istream& is, BtsId& obj);
std::ostream& operator << (std::ostream& os, const BtsId& obj);
std::string to_string(const BtsId& obj);

}
