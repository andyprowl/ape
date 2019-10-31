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

template<typename T>
auto operator == (Position<T> const & lhs, Position<T> const & rhs)
    -> bool
{
    return ((lhs.x == rhs.x) && (lhs.y == rhs.y));
}

template<typename T>
auto operator != (Position<T> const & lhs, Position<T> const & rhs)
    -> bool
{
    return !(lhs == rhs);
}

} // namespace ape
