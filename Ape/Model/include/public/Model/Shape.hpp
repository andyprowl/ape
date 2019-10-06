#pragma once

#include <Model/Vertex.hpp>

#include <GpuResource/ElementBufferObject.hpp>
#include <GpuResource/VertexBufferObject.hpp>

#include <Meta/Counted.hpp>

namespace ape
{

// Shape needs to be assigned a global, unique index that allows fast, vector-based indexing of
// the corresponding VAOs in the given rendering context.
class Shape : public Counted<Shape>
{

public:

    Shape(std::vector<Vertex> const & vertices, std::vector<unsigned int> const & indices);

    auto getVertexBufferObject() const 
        -> const VertexBufferObject &;

    auto getElementBufferObject() const 
        -> const ElementBufferObject &;

    auto getNumOfVertices() const
        -> int;

private:

    class BufferObjects
    {
    
    public:

        VertexBufferObject vertex;

        ElementBufferObject element;
    
    };

private:

    auto makeVertices(
        std::vector<Vertex> const & vertices,
        std::vector<unsigned int> const & indices) const
        -> BufferObjects;

private:
    
    BufferObjects bufferObjects;

    int numOfVertices;

};

} // namespace ape
