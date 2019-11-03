#pragma once

#include <Ape/GpuResource/VertexArrayObject.hpp>
#include <Ape/GpuResource/VertexBufferObject.hpp>

#include <Foundational/Meta/Counted.hpp>

namespace ape
{

class PlainQuad
{

public:

    PlainQuad();

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
