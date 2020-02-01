#pragma once

#include <Ape/World/Shape/ShapeDrawer.hpp>
#include <Ape/World/Shape/ShapeVertex.hpp>

#include <Glow/BufferObject/ElementBufferObject.hpp>
#include <Glow/BufferObject/VertexArrayObject.hpp>
#include <Glow/BufferObject/VertexBufferObject.hpp>

#include <vector>

namespace ape
{

class SharedArrayBufferObjectDrawer : public ShapeDrawer
{

public:

    explicit SharedArrayBufferObjectDrawer(std::vector<Shape *> const & shapes);

    // virtual (from ShapeDrawer)
    auto beginRenderBatch()
        -> void override;

    // virtual (from ShapeDrawer)
    auto render(Shape const & shape)
        -> void override;

    // virtual (from ShapeDrawer)
    auto endRenderBatch()
        -> void override;

private:

    class BufferObjectSet
    {
    
    public:

        glow::VertexArrayObject array;

        glow::VertexBufferObject vertex;

        glow::ElementBufferObject element;
    
    };

    class ShapeBufferOffset
    {

    public:

        ShapeBufferOffset(int const vertexBufferOffset, int const elementBufferOffset)
            : vertexBufferOffset{vertexBufferOffset}
            , elementBufferOffset{elementBufferOffset}
        {
        }

    public:

        int vertexBufferOffset;

        int elementBufferOffset;

    };

    class ShapeGeometry
    {

    public:

        std::vector<ShapeVertex> vertices;

        std::vector<unsigned int> indices;

        std::vector<ShapeBufferOffset> offsets;

    };

private:

    explicit SharedArrayBufferObjectDrawer(ShapeGeometry geometry);

    auto getBufferOffsetsForShape(Shape const & shape) const
        -> const ShapeBufferOffset &;

    auto mergeShapeGeometry(std::vector<Shape *> const & shapes) const
        -> ShapeGeometry;

    auto setupVertexFormat()
        -> void;

private:

    BufferObjectSet bufferObjects;

    std::vector<ShapeBufferOffset> bufferOffsets;

};

} // namespace ape