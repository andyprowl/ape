#pragma once

#include <Basix/Mathematics/Position.hpp>
#include <Basix/Mathematics/Size.hpp>

namespace ape
{

class Viewport
{

public:

    Viewport(basix::Position<int> const & origin, basix::Size<int> const & size)
        : origin{origin}
        , size{size}
    {
    }

public:

    basix::Position<int> origin;

    basix::Size<int> size;

};

} // namespace ape
