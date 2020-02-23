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

    class ShapeBufferPortion
    {

    public:

        ShapeBufferPortion()
            : ShapeBufferPortion{0, 0, 0}
        {
        }

        ShapeBufferPortion(
            int const vertexBufferOffset,
            int const indexBufferByteOffset,
            int const indexBufferElementType)
            : vertexBufferOffset{vertexBufferOffset}
            , indexBufferByteOffset{indexBufferByteOffset}
            , indexBufferElementType{indexBufferElementType}
        {
        }

    public:

        int vertexBufferOffset;

        int indexBufferByteOffset;

        int indexBufferElementType;

    };

    class ShapeGeometry
    {

    public:

        std::vector<std::byte> vertexBufferData;

        std::vector<std::byte> indexBufferData;

        std::vector<ShapeBufferPortion> bufferPortions;

    };

private:

    explicit SharedArrayBufferObjectDrawer(ShapeGeometry geometry);

    auto getBufferPortionForShape(Shape const & shape) const
        -> const ShapeBufferPortion &;

    auto mergeShapeGeometry(std::vector<Shape *> const & shapes) const
        -> ShapeGeometry;

    auto setupVertexFormat()
        -> void;

private:

    BufferObjectSet bufferObjects;

    std::vector<ShapeBufferPortion> bufferPortions;

};

} // namespace ape
