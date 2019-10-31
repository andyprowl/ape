#pragma once

namespace ape
{

template<typename T>
class Offset
{

public:

    Offset(T const deltaX, T const deltaY)
        : deltaX{deltaX}
        , deltaY{deltaY}
    {
    }

public:

    T deltaX;

    T deltaY;

};

template<typename T>
auto operator == (Offset<T> const & lhs, Offset<T> const & rhs)
    -> bool
{
    return ((lhs.deltaX == rhs.deltaX) && (lhs.deltaY == rhs.deltaY));
}

template<typename T>
auto operator != (Offset<T> const & lhs, Offset<T> const & rhs)
    -> bool
{
    return !(lhs == rhs);
}

} // namespace ape
