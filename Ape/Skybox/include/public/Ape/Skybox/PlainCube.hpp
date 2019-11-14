#pragma once

#include <Glow/BufferObject/VertexArrayObject.hpp>
#include <Glow/BufferObject/VertexBufferObject.hpp>

#include <Basix/Meta/Counted.hpp>

namespace ape
{

class PlainCubeVertex;

class PlainCube
{

public:

    PlainCube();

    auto getVertexBuffer() const 
        -> const glow::VertexBufferObject &;

    auto getArrayObject() const 
        -> const glow::VertexArrayObject &;

    auto getNumOfVertices() const
        -> int;

private:

    auto makeVertices() const
        -> glow::VertexBufferObject;

private:

    glow::VertexBufferObject vertexBuffer;

    glow::VertexArrayObject arrayObject;

};

} // namespace ape
