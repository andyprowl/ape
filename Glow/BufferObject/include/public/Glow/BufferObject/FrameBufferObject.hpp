#pragma once

#include <Glow/BufferObject/BufferObject.hpp>
#include <Glow/BufferObject/FrameBufferAttachment.hpp>

#include <string_view>

namespace glow
{

enum class TextureCubeFace;

class RenderBufferObject;
class Texture2d;
class Texture2dArray;
class TextureCube;
class TextureCubeArray;

class FrameBufferObject : public BufferObject
{

public:

    FrameBufferObject();

    explicit FrameBufferObject(std::string_view label);

    auto bind() const
        -> void;

    auto unbind() const
        -> void;

    // WARNING: This is an expensive operation! Use with caution.
    auto isBound() const
        -> bool;

    auto isComplete() const
        -> bool;

    auto attach(Texture2d const & texture, FrameBufferAttachment attachment)
        -> void;

    auto attach(Texture2dArray const & texture, int layer, FrameBufferAttachment attachment)
        -> void;

    auto attach(TextureCube const & texture, FrameBufferAttachment attachment)
        -> void;

    auto attach(
        TextureCube const & texture,
        TextureCubeFace face,
        FrameBufferAttachment attachment)
        -> void;

    auto attach(TextureCubeArray const & texture, int layer, FrameBufferAttachment attachment)
        -> void;

    auto attach(
        TextureCubeArray const & texture,
        int layer,
        TextureCubeFace face,
        FrameBufferAttachment attachment)
        -> void;

    auto attach(RenderBufferObject const & renderBuffer, FrameBufferAttachment attachment)
        -> void;

    auto resetReadTarget()
        -> void;

    auto resetDrawTarget()
        -> void;

    auto setLabel(std::string_view label)
        -> void;

};

} // namespace glow
