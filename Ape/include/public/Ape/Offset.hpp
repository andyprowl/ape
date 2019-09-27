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

} // namespace ape
