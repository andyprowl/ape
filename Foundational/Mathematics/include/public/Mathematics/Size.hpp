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

} // namespace ape
