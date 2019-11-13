#pragma once

#include <Ape/Shape/ShapeDrawer.hpp>

#include <Ape/BufferObject/VertexArrayObject.hpp>

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
        std::vector<VertexArrayObject> & destination) const
        -> void;

    auto setupArrayObjectForShape(
        Shape const & shape,
        VertexArrayObject & destination) const
        -> void;

    auto getArrayObjectForShape(Shape const & shape) const
        -> const VertexArrayObject &;

private:

    std::vector<VertexArrayObject> shapeArrayObjects;

};

} // namespace ape
