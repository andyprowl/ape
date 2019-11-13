#pragma once

#include <Ape/BufferObject/BufferObject.hpp>

#include <Foundational/Mathematics/Size.hpp>

namespace ape
{

class RenderBufferObject : public BufferObject
{

public:
    
    explicit RenderBufferObject(Size<int> const size);

    auto bind() const
        -> void;

    auto unbind() const
        -> void;

    auto setStorage(Size<int> const size)
        -> void;

};

} // namespace ape
