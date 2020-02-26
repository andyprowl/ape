#pragma once

#include <Basix/Mathematics/Position.hpp>
#include <Basix/Mathematics/Size.hpp>

namespace ape
{

class Viewport
{

public:

    Viewport(basix::Position2d<int> const & origin, basix::Size2d<int> const & size)
        : origin{origin}
        , size{size}
    {
    }

public:

    basix::Position2d<int> origin;

    basix::Size2d<int> size;

};

} // namespace ape
