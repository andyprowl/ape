#pragma once

#include <Ape/World/Shape/ShapeDrawer.hpp>

namespace ape
{

class Shape;

class ShapeBufferObjectDrawer : public ShapeDrawer
{

public:

    // virtual (from ShapeDrawer)
    auto beginRenderBatch()
        -> void override;

    // virtual (from ShapeDrawer)
    auto render(Shape const & shape)
        -> void override;

    // virtual (from ShapeDrawer)
    auto endRenderBatch()
        -> void override;

};

} // namespace ape
