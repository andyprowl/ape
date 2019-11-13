#pragma once

#include <Ape/BufferObject/BufferObject.hpp>
#include <Ape/BufferObject/FrameBufferAttachment.hpp>

namespace ape
{

class CubeTexture;
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

    auto isComplete() const
        -> bool;

    auto attach(Texture const & texture, FrameBufferAttachment attachment)
        -> void;

    auto attach(CubeTexture const & texture, FrameBufferAttachment attachment)
        -> void;

    auto attach(RenderBufferObject const & renderBuffer, FrameBufferAttachment attachment)
        -> void;

    auto resetReadTarget()
        -> void;

    auto resetDrawTarget()
        -> void;

};

} // namespace ape
