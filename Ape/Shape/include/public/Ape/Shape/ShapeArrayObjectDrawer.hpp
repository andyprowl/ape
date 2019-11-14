#pragma once

#include <Ape/Shape/ShapeDrawer.hpp>

#include <Glow/BufferObject/VertexArrayObject.hpp>

#include <vector>

namespace ape
{

class Shape;

class ShapeArrayObjectDrawer : public ShapeDrawer
{

public:

    using ShapeCollection = std::vector<Shape *>;

public:

    explicit ShapeArrayObjectDrawer(ShapeCollection const & shapes);

    auto registerShapes(ShapeCollection const & shapes)
        -> void;

    // virtual (from ShapeDrawer)
    auto render(Shape const & shape) const
        -> void override;

private:
    
    auto setupArrayObjectsForShapes(
        ShapeCollection const & shapes,
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
