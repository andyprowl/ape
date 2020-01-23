#pragma once

#include <Ape/World/Shape/ShapeDrawer.hpp>

#include <Glow/BufferObject/ElementBufferObject.hpp>
#include <Glow/BufferObject/VertexArrayObject.hpp>
#include <Glow/BufferObject/VertexBufferObject.hpp>

#include <vector>

namespace ape
{

class Shape;

class ShapeArrayObjectDrawer : public ShapeDrawer
{

public:

    explicit ShapeArrayObjectDrawer(std::vector<Shape *> const & shapes);

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

private:

    auto setupBufferObjectsForShapes(
        std::vector<Shape *> const & shapes,
        std::vector<BufferObjectSet> & destination) const
        -> void;

    auto setupBufferObjectsForShape(
        Shape const & shape,
        BufferObjectSet & destination) const
        -> void;

    auto setupVertexAndElementBuffersForShape(
        Shape const & shape,
        BufferObjectSet & destination) const
        -> void;

    auto setupRenderingState(BufferObjectSet & destination) const
        -> void;

    auto getArrayObjectForShape(Shape const & shape) const
        -> const glow::VertexArrayObject &;

private:

    std::vector<BufferObjectSet> bufferObjects;

};

} // namespace ape
