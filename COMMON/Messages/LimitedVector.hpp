#pragma once

#include <vector>
#include <algorithm> // for std::min

namespace common
{

template <typename ValueType, typename SizeType, SizeType MaxSize>
class LimitedVector : private std::vector<ValueType>
{
    using Impl = std::vector<ValueType>;
public:
    using value_type = typename Impl::value_type;
    using Impl::pointer;
    using Impl::const_pointer;
    using Impl::reference;
    using Impl::const_reference;
    using Impl::difference_type;

    using Impl::iterator;
    using Impl::reverse_iterator;
    using Impl::const_iterator;
    using Impl::const_reverse_iterator;

    using Impl::begin;
    using Impl::cbegin;
    using Impl::end;
    using Impl::cend;
    using Impl::rbegin;
    using Impl::crbegin;
    using Impl::rend;
    using Impl::crend;

    using Impl::operator[];
    using Impl::at;
    using Impl::data;
    using Impl::front;
    using Impl::back;
    using Impl::empty;
    using Impl::size;
    using Impl::clear;
    using Impl::capacity;

    using size_type = SizeType;
    static constexpr size_type max_size() noexcept
    {
        return MaxSize;
    }

    LimitedVector() = default;
    LimitedVector(size_type size, const value_type& value = value_type())
        : Impl(alignSize(size), value)
    {}
    LimitedVector(std::initializer_list<value_type> values) noexcept
        : Impl(values.begin(),
               values.size() > max_size() ? values.begin() + max_size() : values.end())
    {}
    void push_back(const value_type& value) noexcept
    {
        if (size() == max_size())
        {
            return;
        }
        Impl::push_back(value);
    }
    void reserve(size_type size)
    {
        Impl::reserve(alignSize(size));
    }

private:
    static constexpr SizeType alignSize(size_type size) noexcept
    {
        return std::min(size, max_size());
    }
    const Impl& getImpl() const noexcept
    {
        return *this;
    }
    Impl& getImpl() noexcept
    {
        return *this;
    }

    friend inline bool operator == (const LimitedVector& lhs, const LimitedVector& rhs) noexcept
    {
        return lhs.getImpl() == rhs.getImpl();
    }
    friend inline bool operator != (const LimitedVector& lhs, const LimitedVector& rhs) noexcept
    {
        return lhs.getImpl() != rhs.getImpl();
    }
    friend inline bool operator < (const LimitedVector& lhs, const LimitedVector& rhs) noexcept
    {
        return lhs.getImpl() < rhs.getImpl();
    }
    friend inline bool operator > (const LimitedVector& lhs, const LimitedVector& rhs) noexcept
    {
        return lhs.getImpl() > rhs.getImpl();
    }
    friend inline bool operator <= (const LimitedVector& lhs, const LimitedVector& rhs) noexcept
    {
        return lhs.getImpl() <= rhs.getImpl();
    }
    friend inline bool operator >= (const LimitedVector& lhs, const LimitedVector& rhs) noexcept
    {
        return lhs.getImpl() >= rhs.getImpl();
    }
};


}
