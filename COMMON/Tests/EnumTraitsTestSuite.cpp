#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <iostream>
#include "Traits/EnumTraits.hpp"

using namespace ::testing;
namespace common
{

template <class TestEnum>
class EnumTraitsTestSuite : public Test
{
public:
    template <TestEnum Value>
    struct GetName
    {
        constexpr const char* operator()();
    };
    template <TestEnum Value>
    struct PrintName
    {
        std::ostream& operator()(std::ostream& os)
        {
            return os << GetName<Value>{}();
        }
    };
};
TYPED_TEST_SUITE_P(EnumTraitsTestSuite);

TYPED_TEST_P(EnumTraitsTestSuite, forEachShallUseAllEnumValues)
{
    std::ostringstream os;
    EnumRange<TypeParam>::template forEach<EnumTraitsTestSuite<TypeParam>::template PrintName>(os) << "D";

    static_assert(EnumRange<TypeParam>::size() == 3, "Wrong size!");
    ASSERT_EQ("ABCD", os.str());
}
TYPED_TEST_P(EnumTraitsTestSuite, forOneShallUseProperOneEnumValue)
{
    ASSERT_EQ("B", (EnumRange<TypeParam>::template forOne<EnumTraitsTestSuite<TypeParam>::template GetName>(TypeParam::B)));
}
REGISTER_TYPED_TEST_SUITE_P(EnumTraitsTestSuite,
                           forEachShallUseAllEnumValues,
                           forOneShallUseProperOneEnumValue);




enum class EnumTraitsStrictFollower
{
    A,
    B,
    C,

    AfterLast
};
template <> template <>
constexpr const char* EnumTraitsTestSuite<EnumTraitsStrictFollower>::template GetName<EnumTraitsStrictFollower::A>::operator()()
{
    return "A";
}
template <> template <>
constexpr const char* EnumTraitsTestSuite<EnumTraitsStrictFollower>::template GetName<EnumTraitsStrictFollower::B>::operator()()
{
    return "B";
}
template <> template <>
constexpr const char* EnumTraitsTestSuite<EnumTraitsStrictFollower>::template GetName<EnumTraitsStrictFollower::C>::operator()()
{
    return "C";
}

INSTANTIATE_TYPED_TEST_SUITE_P(FollowEnumTraitsScheme, EnumTraitsTestSuite, EnumTraitsStrictFollower);

enum class EnumTraitsFollowerButWithoutLast
{
    A,
    B,
    C,
};
template <> template <>
constexpr const char* EnumTraitsTestSuite<EnumTraitsFollowerButWithoutLast>::template GetName<EnumTraitsFollowerButWithoutLast::A>::operator()()
{
    return "A";
}
template <> template <>
constexpr const char* EnumTraitsTestSuite<EnumTraitsFollowerButWithoutLast>::template GetName<EnumTraitsFollowerButWithoutLast::B>::operator()()
{
    return "B";
}
template <> template <>
constexpr const char* EnumTraitsTestSuite<EnumTraitsFollowerButWithoutLast>::template GetName<EnumTraitsFollowerButWithoutLast::C>::operator()()
{
    return "C";
}

template <>
struct EnumLast<EnumTraitsFollowerButWithoutLast>
        : std::integral_constant<EnumTraitsFollowerButWithoutLast, EnumTraitsFollowerButWithoutLast::C>
{};
INSTANTIATE_TYPED_TEST_SUITE_P(FollowEnumTraitsSchemeButWithoutLast, EnumTraitsTestSuite, EnumTraitsFollowerButWithoutLast);



enum class EnumTraitsNotFollower
{
    A = 1<<1,
    B = 1<<2,
    C = 1<<3,
};
template <>
struct EnumFirst<EnumTraitsNotFollower>
        : std::integral_constant<EnumTraitsNotFollower, EnumTraitsNotFollower::A>
{};
template <>
struct EnumNext<EnumTraitsNotFollower, EnumTraitsNotFollower::A>
        : std::integral_constant<EnumTraitsNotFollower, EnumTraitsNotFollower::B>
{};
template <>
struct EnumNext<EnumTraitsNotFollower, EnumTraitsNotFollower::B>
        : std::integral_constant<EnumTraitsNotFollower, EnumTraitsNotFollower::C>
{};
template <>
struct EnumLast<EnumTraitsNotFollower>
        : std::integral_constant<EnumTraitsNotFollower, EnumTraitsNotFollower::C>
{};



template <> template <>
constexpr const char* EnumTraitsTestSuite<EnumTraitsNotFollower>::template GetName<EnumTraitsNotFollower::A>::operator()()
{
    return "A";
}
template <> template <>
constexpr const char* EnumTraitsTestSuite<EnumTraitsNotFollower>::template GetName<EnumTraitsNotFollower::B>::operator()()
{
    return "B";
}
template <> template <>
constexpr const char* EnumTraitsTestSuite<EnumTraitsNotFollower>::template GetName<EnumTraitsNotFollower::C>::operator()()
{
    return "C";
}

INSTANTIATE_TYPED_TEST_SUITE_P(NotFollowEnumTraits, EnumTraitsTestSuite, EnumTraitsNotFollower);
}
