#pragma once

#include <Ape/World/Shape/ShapeDrawer.hpp>

#include <Glow/BufferObject/VertexArrayObject.hpp>

namespace ape
{

class Shape;

class SharedArrayObjectDrawer : public ShapeDrawer
{

public:

    SharedArrayObjectDrawer();

    // virtual (from ShapeDrawer)
    auto beginRenderBatch()
        -> void override;

    // virtual (from ShapeDrawer)
    auto render(Shape const & shape)
        -> void override;

    // virtual (from ShapeDrawer)
    auto endRenderBatch()
        -> void override;

private:

    glow::VertexArrayObject arrayObject;

};

} // namespace ape
