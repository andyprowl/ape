#pragma once

#include <Rendering/ShapeRenderer.hpp>

#include <GpuResource/VertexArrayObject.hpp>

#include <vector>

namespace ape
{

class Shape;

class ShapeArrayObjectRenderer : public ShapeRenderer
{

public:

    using ShapeCollection = std::vector<Shape *>;

public:

    explicit ShapeArrayObjectRenderer(ShapeCollection const & shapes);

    auto registerShapes(ShapeCollection const & shapes)
        -> void;

    // virtual (from ShapeRenderer)
    auto render(Shape const & shape) const
        -> void override;

private:
    
    auto setupArrayObjectsForShapes(
        ShapeCollection const & shapes,
        std::vector<VertexArrayObject> & destination) const
        -> void;

    auto getArrayObjectForShape(Shape const & shape) const
        -> const VertexArrayObject &;

private:

    std::vector<VertexArrayObject> shapeArrayObjects;

};

} // namespace ape
