#pragma once

#include <Glow/BufferObject/ElementBufferObject.hpp>
#include <Glow/BufferObject/VertexBufferObject.hpp>

namespace ape
{

class Shape;

class ShapeBufferObjectCreator
{

public:

    class BufferObjectSet
    {
    
    public:

        glow::VertexBufferObject vertex;

        glow::ElementBufferObject element;
    
    };

public:

    auto makeBuffers(Shape const & shape) const
        -> BufferObjectSet;

};

} // namespace ape
