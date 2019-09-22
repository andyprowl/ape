#pragma once

namespace ape
{

template<typename T>
class Position
{

public:

    Position(T const x, T const y)
        : x{x}
        , y{y}
    {
    }

public:

    T x;

    T y;

};

} // namespace ape
