#pragma once

#include <Glow/BufferObject/FrameBufferObject.hpp>
#include <Glow/BufferObject/RenderBufferObject.hpp>
#include <Glow/Texture/Texture.hpp>

#include <Basix/Mathematics/Size.hpp>

namespace ape
{

class OffscreenSurface
{

public:

    explicit OffscreenSurface(basix::Size<int> const & size);

    auto getColorBuffer() const
        -> glow::Texture const &;

    auto getDepthAndStencilBuffer() const
        -> glow::RenderBufferObject const &;

    auto getFrameBuffer() const
        -> glow::FrameBufferObject const &;

    auto getSize() const
        -> basix::Size<int>;

    auto setSize(basix::Size<int> const & newSize)
        -> void;

private:

    glow::Texture colorBuffer;

    glow::RenderBufferObject depthAndStencilBuffer;

    glow::FrameBufferObject frameBuffer;

    basix::Size<int> size;

};

} // namespace RenderQuad
