#pragma once

namespace basix
{

template<typename T>
class Offset2d
{

public:

    Offset2d(T const deltaX, T const deltaY)
        : deltaX{deltaX}
        , deltaY{deltaY}
    {
    }

public:

    T deltaX;

    T deltaY;

};

template<typename T>
auto operator == (Offset2d<T> const & lhs, Offset2d<T> const & rhs)
    -> bool
{
    return ((lhs.deltaX == rhs.deltaX) && (lhs.deltaY == rhs.deltaY));
}

template<typename T>
auto operator != (Offset2d<T> const & lhs, Offset2d<T> const & rhs)
    -> bool
{
    return !(lhs == rhs);
}

} // namespace basix
