#pragma once

#include <cstdint>
#include <iostream>
#include <string>

namespace common
{

struct PhoneNumber
{
    using Value = std::uint8_t;
    static constexpr const Value INVALID_VALUE = 0;
    static constexpr const Value MIN_VALUE = 1;
    static constexpr const Value MAX_VALUE = 255;
    static constexpr const std::size_t DIGITS = 3;

    Value value;

    constexpr bool isValid() const { return value >= MIN_VALUE and value <= MAX_VALUE; }
};
inline bool operator == (const PhoneNumber& lhs, const PhoneNumber& rhs)
{
    return lhs.value == rhs.value;
}
inline bool operator != (const PhoneNumber& lhs, const PhoneNumber& rhs)
{
    return !(lhs == rhs);
}

inline bool operator < (const PhoneNumber& lhs, const PhoneNumber& rhs)
{
    return lhs.value < rhs.value;
}
inline bool operator <= (const PhoneNumber& lhs, const PhoneNumber& rhs)
{
    return !(rhs < lhs);
}
inline bool operator >= (const PhoneNumber& lhs, const PhoneNumber& rhs)
{
    return !(lhs < rhs);
}
inline bool operator > (const PhoneNumber& lhs, const PhoneNumber& rhs)
{
    return (rhs < lhs);
}

std::istream& operator >> (std::istream& is, PhoneNumber& obj);
std::ostream& operator << (std::ostream& os, const PhoneNumber& obj);
std::string to_string(const PhoneNumber& obj);

}
