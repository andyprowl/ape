#pragma once

#include <Ape/World/Shape/ShapeBounds.hpp>
#include <Ape/World/Shape/ShapeVertex.hpp>

#include <Glow/BufferObject/ElementBufferObject.hpp>
#include <Glow/BufferObject/VertexBufferObject.hpp>

#include <Basix/Meta/Counted.hpp>

namespace ape
{

// Shape needs to be assigned a global, unique index that allows fast, vector-based indexing of
// the corresponding VAOs in the given rendering context.
class Shape : public basix::Counted<Shape>
{

public:

    Shape(std::vector<ShapeVertex> vertices, std::vector<unsigned int> indices);

    auto getVertices() const
        -> std::vector<ShapeVertex> const &;

    auto getIndices() const
        -> std::vector<unsigned int> const &;

    auto getBoundingVolumes() const
        -> ShapeBounds const &;

private:

    std::vector<ShapeVertex> vertices;

    std::vector<unsigned int> indices;

    ShapeBounds boundingVolumes;

};

inline auto getNumOfVertices(Shape const & shape)
    -> int
{
    return static_cast<int>(shape.getVertices().size());
}

inline auto getNumOfIndices(Shape const & shape)
    -> int
{
    return static_cast<int>(shape.getIndices().size());
}

} // namespace ape
