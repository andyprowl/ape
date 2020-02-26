#pragma once

#include <Glow/BufferObject/BufferObject.hpp>

#include <Basix/Mathematics/Size.hpp>

#include <string_view>

namespace glow
{

class RenderBufferObject : public BufferObject
{

public:
    
    explicit RenderBufferObject(basix::Size2d<int> size);

    RenderBufferObject(basix::Size2d<int> size, std::string_view label);

    auto bind() const
        -> void;

    auto unbind() const
        -> void;

    auto setStorage(basix::Size2d<int> size)
        -> void;

    auto setLabel(std::string_view label)
        -> void;

};

} // namespace glow
