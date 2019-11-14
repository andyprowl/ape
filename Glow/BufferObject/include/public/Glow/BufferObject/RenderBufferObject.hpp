#pragma once

#include <Glow/BufferObject/BufferObject.hpp>

#include <Basix/Mathematics/Size.hpp>

namespace glow
{

class RenderBufferObject : public BufferObject
{

public:
    
    explicit RenderBufferObject(basix::Size<int> const size);

    auto bind() const
        -> void;

    auto unbind() const
        -> void;

    auto setStorage(basix::Size<int> const size)
        -> void;

};

} // namespace glow
