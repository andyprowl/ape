#pragma once

#include <Ape/ElementBufferObject.hpp>
#include <Ape/VertexArrayObject.hpp>
#include <Ape/VertexBufferObject.hpp>
#include <Ape/Vertex.hpp>

#include <vector>

namespace ape
{

class RenderingContext;

class Shape
{

public:

    class BufferSet
    {

    public:

        BufferSet(
            VertexBufferObject vertexBufferObject,
            ElementBufferObject elementBufferObject)
            : vertexBufferObject{std::move(vertexBufferObject)}
            , elementBufferObject{std::move(elementBufferObject)}
        {
        }

    public:

        VertexBufferObject vertexBufferObject;

        ElementBufferObject elementBufferObject;

    };

public:

    Shape(std::vector<Vertex> const & vertices, std::vector<unsigned int> const & indices);

    auto draw(RenderingContext const & context) const
        -> void;

private:

    auto drawWithoutArrayVertexObject() const
        -> void;

    auto drawWithArrayVertexObject(RenderingContext const & context) const
        -> void;

    auto retrieveBoundVertexArrayObject(RenderingContext const & context) const
        -> VertexArrayObject &;

    auto bindExistingVertexArrayObject(RenderingContext const & context) const
        -> VertexArrayObject &;

    auto hasVertexArrayObject(RenderingContext const & context) const
        -> bool;

    auto createNewVertexArrayObject(RenderingContext const & context) const
        -> VertexArrayObject &;

    auto setupRenderingState() const
        -> void;

private:

    BufferSet buffers;

    int numOfVertices;

    mutable std::vector<VertexArrayObject> contextArrayObjectMap;

};

} // namespace ape
