#pragma once

#include <Ape/Model/ShapeVertex.hpp>

#include <Ape/GpuResource/ElementBufferObject.hpp>
#include <Ape/GpuResource/VertexArrayObject.hpp>
#include <Ape/GpuResource/VertexBufferObject.hpp>

#include <Foundational/Meta/Counted.hpp>

namespace ape
{

class FlatQuad
{

public:

    FlatQuad();

    auto getVertexBuffer() const 
        -> const VertexBufferObject &;

    auto getArrayObject() const 
        -> const VertexArrayObject &;

    auto getNumOfVertices() const
        -> int;

private:

    auto makeVertices() const
        -> VertexBufferObject;

private:

    VertexBufferObject vertexBuffer;

    VertexArrayObject arrayObject;

};

} // namespace ape
