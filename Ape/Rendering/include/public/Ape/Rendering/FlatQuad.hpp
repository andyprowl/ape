#pragma once

#include <Ape/Model/ShapeVertex.hpp>

#include <Ape/GpuResource/ElementBufferObject.hpp>
#include <Ape/GpuResource/VertexBufferObject.hpp>

#include <Foundational/Meta/Counted.hpp>

namespace ape
{

class FlatQuad
{

public:

    FlatQuad();

    auto getVertexBufferObject() const 
        -> const VertexBufferObject &;

    auto getNumOfVertices() const
        -> int;

private:

    auto makeVertices() const
        -> VertexBufferObject;

private:
    
    VertexBufferObject vertexBuffer;

};

} // namespace ape
