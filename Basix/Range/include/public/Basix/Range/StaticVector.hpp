#pragma once

#include <array>
#include <new>

namespace basix
{

template<typename T, int Capacity>
class StaticVector
{

public:

    using AlignedStorage = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

    using Storage = std::array<AlignedStorage, Capacity>;

    using value_type = T;

    using iterator = T *;

    using const_iterator = T const *;

public:

    static constexpr auto capacity()
        -> int
    {
        return Capacity;
    }

    StaticVector()
        : currentSize{0}
        , storage{}
    {
    }

    StaticVector(std::initializer_list<T> values)
        : currentSize{0}
        , storage{}
    {
        for (auto const & value : values)
        {
            push_back(value);
        }
    }

    StaticVector(StaticVector const & rhs)
        : currentSize{rhs.currentSize}
        , storage{rhs.storage}
    {
        for (auto i = 0; i < currentSize; ++i)
        {
            new (&storage[i]) T{rhs[i]};
        }
    }

    StaticVector(StaticVector && rhs) noexcept
        : currentSize{rhs.currentSize}
        , storage{rhs.storage}
    {
        for (auto i = 0; i < currentSize; ++i)
        {
            new (&storage[i]) T{std::move(rhs[i])};
        }
    }

    auto operator = (StaticVector const & rhs)
        -> StaticVector &
    {
        destroyAllElements();

        for (auto i = 0; i < currentSize; ++i)
        {
            push_back(rhs[i]);
        }

        return *this;
    }

    auto operator = (StaticVector && rhs) noexcept
        -> StaticVector &
    {
        destroyAllElements();

        for (auto i = 0; i < currentSize; ++i)
        {
            push_back(std::move(rhs[i]));
        }

        return *this;
    }

    ~StaticVector()
    {
        destroyAllElements();
    }

    auto size() const
        -> int
    {
        return currentSize;
    }

    auto empty() const
        -> bool
    {
        return (size() == 0);
    }

    auto push_back(T const & value)
        -> void
    {
        throwIfFull();

        new (&storage[currentSize++]) T{value};
    }

    auto push_back(T && value)
        -> void
    {
        throwIfFull();

        new (&storage[currentSize++]) T{std::move(value)};
    }

    auto clear()
        -> void
    {
        destroyAllElements();

        currentSize = 0;
    }

    auto front()
        -> T &
    {
        return (*this)[0];
    }

    auto front() const
        -> T const &
    {
        return (*this)[0];
    }

    auto back()
        -> T &
    {
        return (*this)[currentSize - 1];
    }

    auto back() const
        -> T const &
    {
        return (*this)[currentSize - 1];
    }

    auto at(int const i)
        -> T &
    {
        throwIfOutOfRange(i);

        return (*this)[i];
    }

    auto at(int const i) const
        -> T const &
    {
        throwIfOutOfRange(i);

        return (*this)[i];
    }

    auto operator [] (int const i)
        -> T &
    {
        auto const p = reinterpret_cast<T *>(&storage[i]);

        return *(std::launder(p));
    }

    auto operator [] (int const i) const
        -> T const &
    {
        auto const p = reinterpret_cast<T const *>(&storage[i]);

        return *(std::launder(p));
    }

    auto begin()
        -> iterator
    {
        return reinterpret_cast<T *>(storage.data());
    }

    auto begin() const
        -> const_iterator
    {
        return reinterpret_cast<T const *>(storage.data());
    }

    auto end()
        -> iterator
    {
        return reinterpret_cast<T *>(storage.data() + currentSize);
    }

    auto end() const
        -> const_iterator
    {
        return reinterpret_cast<T const *>(storage.data() + currentSize);
    }

private:

    auto throwIfFull() const
        -> void
    {
        if (size() == capacity())
        {
            throw std::out_of_range{"Capacity exceeded"};
        }
    }

    auto throwIfOutOfRange(int const i) const
        -> void
    {
        if ((i < 0) || (i >= size()))
        {
            throw std::out_of_range{"Out-of-bounds access"};
        }
    }

    auto destroyAllElements()
        -> void
    {
        for (auto i = 0; i < currentSize; ++i)
        {
            auto const p = std::launder(reinterpret_cast<T *>(&storage[i]));
            
            p->~T();
        }
    }

private:

    int currentSize;

    Storage storage;

};

} // namespace basix
