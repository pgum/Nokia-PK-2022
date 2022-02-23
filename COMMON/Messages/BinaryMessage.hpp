#pragma once

#include <cstdint>
#include <iostream>
#include <limits>
#include "LimitedVector.hpp"

namespace common
{

constexpr std::size_t max_size_min(std::size_t a, std::size_t b)
{
    return (a < b) ? a : b;
}

struct BinaryMessage
{
    using ValueType = std::uint8_t;
    using SizeType = std::uint16_t; // used in transport
    // for bigger types than uint8_t - consider to use other value than max (lower)
    static constexpr std::size_t MAX_SIZE = max_size_min(5000, std::numeric_limits<SizeType>::max());

    using Value = LimitedVector<ValueType, SizeType, MAX_SIZE>;

    Value value;
};

std::ostream& operator << (std::ostream& os, const BinaryMessage& message);
std::istream& operator >> (std::istream& is, BinaryMessage& message);


}
