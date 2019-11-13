#pragma once

#include <Ape/BufferObject/VertexArrayObject.hpp>
#include <Ape/BufferObject/VertexBufferObject.hpp>

#include <Foundational/Meta/Counted.hpp>

namespace ape
{

class PlainCubeVertex;

class PlainCube
{

public:

    PlainCube();

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
