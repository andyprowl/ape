#pragma once

#include <GpuResource/BufferObject.hpp>
#include <GpuResource/FrameBufferAttachment.hpp>

namespace ape
{

class RenderBufferObject;
class Texture;

class FrameBufferObject : public BufferObject
{

public:

    FrameBufferObject();

    auto bind() const
        -> void;

    auto unbind() const
        -> void;

    // WARNING: This is an expensive operation! Use with caution.
    auto isBound() const
        -> bool;

    auto isComplete(bool bind = true) const
        -> bool;

    auto attach(Texture const & texture, FrameBufferAttachment attachment)
        -> void;

    auto attach(RenderBufferObject const & renderBuffer, FrameBufferAttachment attachment)
        -> void;

};

} // namespace ape
