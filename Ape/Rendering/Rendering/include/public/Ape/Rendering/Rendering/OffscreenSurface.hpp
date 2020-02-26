#pragma once

#include <Glow/BufferObject/FrameBufferObject.hpp>
#include <Glow/BufferObject/RenderBufferObject.hpp>
#include <Glow/Texture/Texture2d.hpp>

#include <Basix/Mathematics/Size.hpp>

namespace ape
{

class OffscreenSurface
{

public:

    explicit OffscreenSurface(basix::Size2d<int> const & size);

    auto getColorBuffer() const
        -> glow::Texture2d const &;

    auto getDepthAndStencilBuffer() const
        -> glow::RenderBufferObject const &;

    auto getFrameBuffer() const
        -> glow::FrameBufferObject const &;

    auto getSize() const
        -> basix::Size2d<int>;

private:

    glow::Texture2d colorBuffer;

    glow::RenderBufferObject depthAndStencilBuffer;

    glow::FrameBufferObject frameBuffer;

    basix::Size2d<int> size;

};

} // namespace RenderQuad
