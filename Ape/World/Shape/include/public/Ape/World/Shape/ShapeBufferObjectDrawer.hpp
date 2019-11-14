#pragma once

#include <Ape/World/Shape/ShapeDrawer.hpp>

namespace ape
{

class Shape;

class ShapeBufferObjectDrawer : public ShapeDrawer
{

public:

    // virtual (from ShapeDrawer)
    auto render(Shape const & shape) const
        -> void override;

};

} // namespace ape
