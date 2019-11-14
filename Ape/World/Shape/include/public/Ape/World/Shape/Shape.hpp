#pragma once

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

    Shape(std::vector<ShapeVertex> const & vertices, std::vector<unsigned int> const & indices);

    auto getVertexBufferObject() const
        -> const glow::VertexBufferObject &;

    auto getElementBufferObject() const
        -> const glow::ElementBufferObject &;

    auto getNumOfVertices() const
        -> int;

private:

    class BufferObjects
    {
    
    public:

        glow::VertexBufferObject vertex;

        glow::ElementBufferObject element;
    
    };

private:

    auto makeVertices(
        std::vector<ShapeVertex> const & vertices,
        std::vector<unsigned int> const & indices) const
        -> BufferObjects;

private:
    
    BufferObjects bufferObjects;

    int numOfVertices;

};

} // namespace ape
