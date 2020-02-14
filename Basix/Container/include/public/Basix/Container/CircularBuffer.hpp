#pragma once

#include <Basix/Container/CircularBufferIterator.hpp>

#include <stdexcept>
#include <vector>

namespace basix
{

template<typename T>
class CircularBuffer
{

public:

    using value_type = T;

    using iterator = CircularBufferIterator<T>;

    using const_iterator = CircularBufferIterator<T const>;

public:

    explicit CircularBuffer(int const maxSize)
        : storage{new AlignedStorage[static_cast<std::size_t>(maxSize) + 1u]}
        , numOfElements{0}
        , maxNumOfElements{maxSize}
        , firstIndex{0}
    {
    }

    CircularBuffer(CircularBuffer && rhs) noexcept
        : storage{std::exchange(rhs.storage, nullptr)}
        , numOfElements{std::exchange(rhs.numOfElements, 0)}
        , maxNumOfElements{std::exchange(rhs.maxNumOfElements, 0)}
        , firstIndex{std::exchange(rhs.firstIndex, 0)}
    {
    }

    auto operator = (CircularBuffer && rhs) noexcept
        -> CircularBuffer &
    {
        storage = std::exchange(rhs.storage, nullptr);
        
        numOfElements = std::exchange(rhs.numOfElements, 0);
        
        maxNumOfElements = std::exchange(rhs.maxNumOfElements, 0);
        
        firstIndex = std::exchange(rhs.firstIndex, 0);

        return *this;
    }

    ~CircularBuffer()
    {
        destroyAllElements();

        delete[] storage;
    }

    auto size() const
        -> int
    {
        return numOfElements;
    }

    auto capacity() const
        -> int
    {
        return maxNumOfElements;
    }

    auto empty() const
        -> bool
    {
        return (size() == 0);
    }

    auto full() const
        -> bool
    {
        return (size() == maxNumOfElements);
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
        destroyAllElements();

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
        auto const p = reinterpret_cast<value_type const *>(&storage[firstIndex]);

        return *(std::launder(p));
    }

    auto back()
        -> value_type &
    {
        return const_cast<value_type &>(asConst().back());
    }

    auto back() const
        -> value_type const &
    {
        auto const storageSize = maxNumOfElements + 1;

        auto const lastIndex = (firstIndex + (numOfElements - 1)) % storageSize;

        auto const p = reinterpret_cast<value_type const *>(&storage[lastIndex]);

        return *(std::launder(p));
    }

    auto at(int const i)
        -> value_type &
    {
        return const_cast<value_type &>(asConst().at(i));
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
        auto const storageSize = maxNumOfElements + 1;

        auto const index = (firstIndex + i) % storageSize;

        auto const p = reinterpret_cast<value_type const *>(&storage[index]);

        return *(std::launder(p));
    }

    auto begin()
        -> iterator
    {
        return const_cast<iterator>(asConst().begin());
    }

    auto begin() const
        -> const_iterator
    {
        auto const storageSize = maxNumOfElements + 1;

        return {storage, storageSize, firstIndex};
    }

    auto end()
        -> iterator
    {
        return const_cast<iterator>(asConst().end());
    }

    auto end() const
        -> const_iterator
    {
        auto const storageSize = maxNumOfElements + 1;

        auto const onePastLastElementIndex = (firstIndex + numOfElements) % storageSize;

        return {storage, storageSize, onePastLastElementIndex};
    }

    auto data()
        -> value_type *
    {
        return const_cast<value_type *>(asConst().data());
    }

    auto data() const
        -> value_type const *
    {
        auto const p = reinterpret_cast<value_type const *>(&storage[0]);

        return std::launder(p);
    }

private:

    using AlignedStorage = std::aligned_storage_t<sizeof(value_type), alignof(value_type)>;

private:

    template<typename T>
    auto insert(T && value)
        -> void
    {
        if (not full())
        {
            new (&storage[numOfElements++]) T{std::forward<T>(value)};
        }
        else
        {
            auto const storageSize = maxNumOfElements + 1;

            auto const insertionIndex = (firstIndex + maxNumOfElements) % storageSize;

            new (&storage[insertionIndex]) T{std::forward<T>(value)};

            auto const first = std::launder(reinterpret_cast<T *>(&storage[firstIndex]));
            
            first->~value_type();

            firstIndex = (firstIndex + 1) % storageSize;
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

    auto destroyAllElements()
        -> void
    {
        for (auto i = firstIndex; i < numOfElements; ++i)
        {
            auto const index = i % maxNumOfElements;

            auto const p = std::launder(reinterpret_cast<T *>(&storage[index]));
            
            p->~value_type();
        }
    }

private:

    AlignedStorage * storage;

    int numOfElements;

    int maxNumOfElements;

    int firstIndex;

};

} // namespace basix
