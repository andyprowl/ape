#pragma once

#include <cassert>
#include <iterator>
#include <type_traits>
#include <utility>

namespace basix
{

template<typename T>
class CircularBufferIterator
{

public:

    using AlignedStorage = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

    using value_type = T;

    using difference_type = ptrdiff_t;

    using reference = T &;
    
    using pointer = T *;

    using iterator_category = std::random_access_iterator_tag;

public:

    CircularBufferIterator(AlignedStorage * const storage, int const storageSize, int const index)
        : storage{storage}
        , storageSize{storageSize}
        , index{index}
    {
    }

    auto operator * ()
        -> T &
    {
        return const_cast<T &>(*(asConst()));
    }

    auto operator * () const
        -> T const &
    {
        auto const p = reinterpret_cast<value_type const *>(&storage[index]);

        return *(std::launder(p));
    }

    auto operator -> ()
        -> T *
    {
        return const_cast<T *>(asConst().operator -> ());
    }

    auto operator -> () const
        -> T const *
    {
        return &(*this);
    }

    auto operator ++ ()
        -> CircularBufferIterator &
    {
        index = (index + 1) % storageSize;

        return *this;
    }

    auto operator -- ()
        -> CircularBufferIterator &
    {
        index = (index + storageSize - 1) % storageSize;

        return *this;
    }

    auto operator + (std::size_t const n) const
        -> CircularBufferIterator
    {
        auto const newIndex = (index + (n % storageSize) + storageSize) % storageSize;

        return {storage, storageSize, newIndex};
    }

    auto operator - (std::size_t const n) const
        -> CircularBufferIterator
    {
        return (*this + (-n));
    }

    template<typename U>
    auto operator - (CircularBufferIterator<U> const rhs) const
        -> difference_type
    {
        if (index >= rhs.index)
        {
            return (index - rhs.index);
        }
        else
        {
            return (rhs.index + (storageSize - index));
        }
    }

    template<typename U>
    auto operator == (CircularBufferIterator<U> const & rhs) const
        -> bool
    {
        static_assert(std::is_same_v<std::decay_t<T>, std::decay_t<U>>);

        assert(storage == rhs.storage);

        return (index == rhs.index);
    }

    template<typename U>
    auto operator != (CircularBufferIterator<U> const & rhs) const
        -> bool
    {
        static_assert(std::is_same_v<std::decay_t<T>, std::decay_t<U>>);

        assert(storage == rhs.storage);

        return (index != rhs.index);
    }

private:

    auto asConst() const
        -> CircularBufferIterator const &
    {
        return *this;
    }

private:

    AlignedStorage * storage;

    int storageSize;

    int index;

};

} // namespace basix
