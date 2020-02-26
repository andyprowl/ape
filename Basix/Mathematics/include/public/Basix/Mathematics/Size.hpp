#pragma once

namespace basix
{

template<typename T>
class Size2d
{

public:

    Size2d(T const width, T const height)
        : width{width}
        , height{height}
    {
    }

public:

    T width;

    T height;

};

template<typename T>
auto operator == (Size2d<T> const & lhs, Size2d<T> const & rhs)
    -> bool
{
    return ((lhs.width == rhs.width) && (lhs.height == rhs.height));
}

template<typename T>
auto operator != (Size2d<T> const & lhs, Size2d<T> const & rhs)
    -> bool
{
    return !(lhs == rhs);
}

template<typename T>
auto operator * (Size2d<T> const & size, T const factor)
    -> Size2d<T>
{
    return {size.width * factor, size.height * factor};
}

template<typename T>
auto operator / (Size2d<T> const & size, T const factor)
    -> Size2d<T>
{
    return {size.width / factor, size.height / factor};
}

template<typename T>
auto operator + (Size2d<T> const & lhs, Size2d<T> const & rhs)
    -> Size2d<T>
{
    return {lhs.width + rhs.width, lhs.height + rhs.height};
}

template<typename T>
auto operator - (Size2d<T> const & lhs, Size2d<T> const & rhs)
    -> Size2d<T>
{
    return {lhs.width - rhs.width, lhs.height - rhs.height};
}

} // namespace basix
