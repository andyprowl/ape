#pragma once

#include <Ape/World/Shape/ShapeDrawer.hpp>

#include <Glow/BufferObject/ElementBufferObject.hpp>
#include <Glow/BufferObject/VertexBufferObject.hpp>

#include <vector>

namespace ape
{

class Shape;

class ShapeBufferObjectDrawer : public ShapeDrawer
{

public:

    explicit ShapeBufferObjectDrawer(std::vector<Shape *> const & shapes);

    // virtual (from ShapeDrawer)
    auto beginDrawBatch()
        -> void override;

    // virtual (from ShapeDrawer)
    auto draw(Shape const & shape)
        -> void override;

    // virtual (from ShapeDrawer)
    auto endDrawBatch()
        -> void override;

public:

    class BufferObjectSet
    {
    
    public:

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

    auto getBufferObjectsForShape(Shape const & shape) const
        -> const BufferObjectSet &;

private:

    std::vector<BufferObjectSet> bufferObjects;

};

} // namespace ape
