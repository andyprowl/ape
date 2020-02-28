#include <Ape/Rendering/Rendering/OffscreenSurface.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>

#include <cassert>

namespace ape
{

namespace
{

auto makeColorBuffer(basix::Size2d<int> const & size)
    -> glow::Texture2d
{
    auto const descriptor = glow::Texture2dDescriptor{
        size,
        glow::TextureInternalFormat::srgb8,
        glow::TextureFiltering{
            glow::TextureMagnificationFilter::linear,
            glow::TextureMinificationFilter::linear},
        glow::TextureWrapping::clampToEdge};

    return glow::Texture2d{descriptor, "Offscreen surface color buffer"};
}

auto makeDepthAndStencilBuffer(basix::Size2d<int> const & size)
    -> glow::RenderBufferObject
{
    return glow::RenderBufferObject{size, "Offscreen surface render buffer"};
}

auto makeFrameBufferForTarget(
    glow::Texture2d & colorBuffer,
    glow::RenderBufferObject & depthAndStencilBuffer)
    -> glow::FrameBufferObject
{
    auto frameBuffer = glow::FrameBufferObject{"Offscreen surface framebuffer"};

    frameBuffer.attach(colorBuffer, glow::FrameBufferAttachment::color0);
    
    frameBuffer.attach(depthAndStencilBuffer, glow::FrameBufferAttachment::depthAndStencil);

    assert(frameBuffer.isComplete());

    return frameBuffer;
}

} // unnamed namespace

OffscreenSurface::OffscreenSurface(basix::Size2d<int> const & size)
    : colorBuffer{makeColorBuffer(size)}
    , depthAndStencilBuffer{makeDepthAndStencilBuffer(size)}
    , frameBuffer{makeFrameBufferForTarget(colorBuffer, depthAndStencilBuffer)}
    , size{size}
{
}

auto OffscreenSurface::getColorBuffer() const
    -> glow::Texture2d const &
{
    return colorBuffer;
}

auto OffscreenSurface::getDepthAndStencilBuffer() const
    -> glow::RenderBufferObject const &
{
    return depthAndStencilBuffer;
}

auto OffscreenSurface::getFrameBuffer() const
    -> glow::FrameBufferObject const &
{
    return frameBuffer;
}

auto OffscreenSurface::getSize() const
    -> basix::Size2d<int>
{
    return size;
}

} // namespace ape
