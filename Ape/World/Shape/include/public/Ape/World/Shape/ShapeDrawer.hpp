#pragma once

namespace ape
{

class Shape;

class ShapeDrawer
{

public:

    virtual ~ShapeDrawer() = default;

    virtual auto beginRenderBatch()
        -> void = 0;

    virtual auto render(Shape const & shape)
        -> void = 0;

    virtual auto endRenderBatch()
        -> void = 0;

};

} // namespace ape
