#pragma once

namespace basix
{

template<typename T>
class Position2d
{

public:

    Position2d(T const x, T const y)
        : x{x}
        , y{y}
    {
    }

public:

    T x;

    T y;

};

template<typename T>
auto operator == (Position2d<T> const & lhs, Position2d<T> const & rhs)
    -> bool
{
    return ((lhs.x == rhs.x) && (lhs.y == rhs.y));
}

template<typename T>
auto operator != (Position2d<T> const & lhs, Position2d<T> const & rhs)
    -> bool
{
    return !(lhs == rhs);
}

} // namespace basix
