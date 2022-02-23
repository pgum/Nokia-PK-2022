#pragma once

#include <type_traits>
#include <utility>
#include <stdexcept>

namespace common
{

template <typename Enum, typename Return=std::underlying_type_t<Enum>>
constexpr Return enumUnderlyingValue(Enum value)
{
    return static_cast<Return>(value);
}
template <typename Enum, typename Value = std::underlying_type_t<Enum>>
constexpr Enum enumValue(Value value)
{
    return static_cast<Enum>(value);
}

template <typename Enum>
struct EnumFirst : std::integral_constant<Enum, enumValue<Enum>(0u)>
{};
template <typename Enum>
struct EnumLast : std::integral_constant<Enum, enumValue<Enum>(enumUnderlyingValue(Enum::AfterLast) - 1)>
{};
template <typename Enum, Enum Current>
struct EnumNext : std::integral_constant<Enum, enumValue<Enum>(enumUnderlyingValue(Current) + 1)>
{};

template <typename Enum, Enum First = EnumFirst<Enum>::value, Enum Last = EnumLast<Enum>::value>
class EnumRange;

template <typename Enum, Enum Value>
class EnumRange<Enum, Value, Value>
{
public:
    template <template <Enum> class Functor, typename ...Args>
    static constexpr decltype(auto) forEach(Args&& ...args);
    template <template <Enum> class Functor, typename ...Args>
    static /*constexpr*/ decltype(auto) forOne(Enum theOne, Args&& ...args);
    static constexpr std::size_t size() { return 1u; }
};

template <typename Enum, Enum First, Enum Last>
class EnumRange
{
public:
    template <template <Enum> class Functor, typename ...Args>
    static constexpr decltype(auto) forEach(Args&& ...args);
    template <template <Enum> class Functor, typename ...Args>
    static constexpr decltype(auto) forOne(Enum theOne, Args&& ...args);
    static constexpr std::size_t size() { return ForFirst::size() + ForSecondToLast::size(); }
private:
    using ForFirst = EnumRange<Enum, First, First>;
    using ForSecondToLast = EnumRange<Enum, EnumNext<Enum, First>::value, Last>;
};

template <typename Enum, Enum Value>
template <template <Enum> class Functor, typename ...Args>
constexpr decltype(auto) EnumRange<Enum, Value, Value>::forEach(Args&& ...args)
{
    return Functor<Value>{}(std::forward<Args>(args)...);
};

template <typename Enum, Enum Value>
template <template <Enum> class Functor, typename ...Args>
/*constexpr*/ decltype(auto) EnumRange<Enum, Value, Value>::forOne(Enum theOne, Args&& ...args)
{
    return (Value == theOne) ? EnumRange<Enum, Value, Value>::template forEach<Functor>(std::forward<Args>(args)...)
                             : throw std::out_of_range(std::to_string(enumUnderlyingValue(theOne))
                                                       + "!="
                                                       +  std::to_string(enumUnderlyingValue(Value)));
};

template <typename Enum, Enum First, Enum Last>
template <template <Enum> class Functor, typename ...Args>
constexpr decltype(auto) EnumRange<Enum, First, Last>::forEach(Args&& ...args)
{
    return ForFirst::template forEach<Functor>(std::forward<Args>(args)...),
           ForSecondToLast::template forEach<Functor>(std::forward<Args>(args)...);
};

template <typename Enum, Enum First, Enum Last>
template <template <Enum> class Functor, typename ...Args>
constexpr decltype(auto) EnumRange<Enum, First, Last>::forOne(Enum theOne, Args&& ...args)
{
    return (First == theOne) ? ForFirst::template forEach<Functor>(std::forward<Args>(args)...)
                             : ForSecondToLast::template forOne<Functor>(theOne, std::forward<Args>(args)...);
};

}
