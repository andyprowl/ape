#pragma once

namespace ape
{

template<typename T>
class Size
{

public:

    Size(T const width, T const height)
        : width{width}
        , height{height}
    {
    }

public:

    T width;

    T height;

};

template<typename T>
auto operator == (Size<T> const & lhs, Size<T> const & rhs)
    -> bool
{
    return ((lhs.width == rhs.width) && (lhs.height == rhs.height));
}

template<typename T>
auto operator != (Size<T> const & lhs, Size<T> const & rhs)
    -> bool
{
    return !(lhs == rhs);
}

template<typename T>
auto operator * (Size<T> const & size, T const factor)
    -> Size<T>
{
    return {size.width * factor, size.height * factor};
}

template<typename T>
auto operator / (Size<T> const & size, T const factor)
    -> Size<T>
{
    return {size.width / factor, size.height / factor};
}

template<typename T>
auto operator + (Size<T> const & lhs, Size<T> const & rhs)
    -> Size<T>
{
    return {lhs.width + rhs.width, lhs.height + rhs.height};
}

template<typename T>
auto operator - (Size<T> const & lhs, Size<T> const & rhs)
    -> Size<T>
{
    return {lhs.width - rhs.width, lhs.height - rhs.height};
}

} // namespace ape
