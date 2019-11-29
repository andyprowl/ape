#pragma once

#include <stdexcept>
#include <vector>

namespace basix
{

template<typename T>
class CircularBuffer
{

public:

    using value_type = T;

    using iterator = T *;

    using const_iterator = T const *;

public:

    CircularBuffer(std::size_t const maxSize)
        : maxSize{maxSize}
        , firstIndex{0}
    {
        storage.reserve(maxSize);
    }

    auto size() const
        -> int
    {
        return static_cast<int>(storage.size());
    }

    auto empty() const
        -> bool
    {
        return (size() == 0);
    }

    auto full() const
        -> bool
    {
        return (size() == maxSize);
    }

    auto push_back(value_type const & value)
        -> void
    {
        return insert(value);
    }

    auto push_back(value_type && value)
        -> void
    {
        return insert(std::move(value));
    }

    auto clear()
        -> void
    {
        storage.clear();

        firstIndex = 0;
    }

    auto front()
        -> value_type &
    {
        return const_cast<value_type &>(asConst().front());
    }

    auto front() const
        -> value_type const &
    {
        if (full())
        {
            return storage[firstIndex];
        }
        else
        {
            return storage.back();
        }
    }

    auto back()
        -> value_type &
    {
        return const_cast<value_type &>(asConst().back());
    }

    auto back() const
        -> value_type const &
    {
        auto const lastElementIndex = (firstIndex + maxSize - 1) % maxSize;

        return storage[lastElementIndex];
    }

    auto at(int const i)
        -> value_type &
    {
        throwIfOutOfRange(i);

        return (*this)[i];
    }

    auto at(int const i) const
        -> value_type const &
    {
        throwIfOutOfRange(i);

        return (*this)[i];
    }

    auto operator [] (int const i)
        -> value_type &
    {
        return const_cast<value_type &>(asConst()[i]);
    }

    auto operator [] (int const i) const
        -> value_type const &
    {
        auto const index = (firstIndex + i) % maxSize;

        return storage[index];
    }

    auto begin()
        -> iterator
    {
        return const_cast<iterator>(asConst().begin());
    }

    auto begin() const
        -> const_iterator
    {
        return std::next(std::begin(storage), firstIndex);
    }

    auto end()
        -> iterator
    {
        return const_cast<iterator>(asConst().end());
    }

    auto end() const
        -> const_iterator
    {
        auto const lastElementIndex = (firstIndex + maxSize - 1) % maxSize;

        return std::next(std::begin(storage), lastElementIndex);
    }

private:

    template<typename T>
    auto insert(T && value)
        -> void
    {
        if (!full())
        {
            storage.push_back(value);
        }
        else
        {
            storage[firstIndex] = value;

            firstIndex = (firstIndex + 1) % maxSize;
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

    auto asConst() const
        -> CircularBuffer const &
    {
        return *this;
    }

private:

    int maxSize;

    int firstIndex;

    std::vector<T> storage;

};

} // namespace basix
