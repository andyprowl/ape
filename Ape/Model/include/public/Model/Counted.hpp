#pragma once

#include <vector>

namespace ape
{

// The Classifier template parameter allows creating two different instances of Counted
// to be used for several different types of counted objects.
template<typename Classifier>
class Counted
{

public:

    static auto getNumOfInstances()
        -> int
    {
        return numOfInstances;
    }

public:

    Counted()
        : index{numOfInstances++}
    {
    }

    auto getInstanceIndex() const
        -> int
    {
        return index;
    }

private:

    static inline auto numOfInstances = 0;

private:

    int index;

};

} // namespace ape
