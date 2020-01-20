#pragma once

#include <Ape/World/Shape/ShapeDrawer.hpp>

#include <Glow/BufferObject/VertexArrayObject.hpp>

#include <vector>

namespace ape
{

class Shape;

class ShapeArrayObjectDrawer : public ShapeDrawer
{

public:

    explicit ShapeArrayObjectDrawer(std::vector<Shape *> const & shapes);

    auto registerShapes(std::vector<Shape *> const & shapes)
        -> void;

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
    
    auto setupArrayObjectsForShapes(
        std::vector<Shape *> const & shapes,
        std::vector<glow::VertexArrayObject> & destination) const
        -> void;

    auto setupArrayObjectForShape(
        Shape const & shape,
        glow::VertexArrayObject & destination) const
        -> void;

    auto getArrayObjectForShape(Shape const & shape) const
        -> const glow::VertexArrayObject &;

private:

    std::vector<glow::VertexArrayObject> shapeArrayObjects;

};

} // namespace ape
