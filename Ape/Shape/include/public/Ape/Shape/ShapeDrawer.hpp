#pragma once

namespace ape
{

class Shape;

class ShapeDrawer
{

public:

    virtual ~ShapeDrawer() = default;

    virtual auto render(Shape const & shape) const
        -> void = 0;

};

} // namespace ape
