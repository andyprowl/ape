#pragma once

#include <Mathematics/Position.hpp>
#include <Mathematics/Size.hpp>

namespace ape
{

class Viewport
{

public:

    Viewport(Position<int> const & origin, Size<int> const & size)
        : origin{origin}
        , size{size}
    {
    }

public:

    Position<int> origin;

    Size<int> size;

};

} // namespace ape
