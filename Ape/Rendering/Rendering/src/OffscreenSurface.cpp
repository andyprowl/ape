#include <Ape/Rendering/Rendering/OffscreenSurface.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>
#include <Glow/Texture/TextureStorageType.hpp>

#include <cassert>

namespace ape
{

namespace
{

auto makeColorBuffer(basix::Size<int> const & size)
    -> glow::Texture
{
    auto image = glow::TextureImage{
        nullptr,
        size,
        glow::TextureImageFormat::rgb,
        glow::PixelType::unsignedByte};

    auto const descriptor = glow::TextureDescriptor{
        std::move(image),
        glow::TextureInternalFormat::srgb8,
        glow::TextureWrapping::clampToEdge,
        glow::TextureStorageType::modifiable};

    return glow::Texture{descriptor};
}

auto makeDepthAndStencilBuffer(basix::Size<int> const & size)
    -> glow::RenderBufferObject
{
    return glow::RenderBufferObject{size};
}

auto makeFrameBufferForTarget(
    glow::Texture & colorBuffer,
    glow::RenderBufferObject & depthAndStencilBuffer)
    -> glow::FrameBufferObject
{
    auto frameBuffer = glow::FrameBufferObject{};

    auto const binder = glow::bind(frameBuffer);

    frameBuffer.attach(colorBuffer, glow::FrameBufferAttachment::color0);
    
    frameBuffer.attach(depthAndStencilBuffer, glow::FrameBufferAttachment::depthAndStencil);

    assert(frameBuffer.isComplete());

    return frameBuffer;
}

} // unnamed namespace

OffscreenSurface::OffscreenSurface(basix::Size<int> const & size)
    : colorBuffer{makeColorBuffer(size)}
    , depthAndStencilBuffer{makeDepthAndStencilBuffer(size)}
    , frameBuffer{makeFrameBufferForTarget(colorBuffer, depthAndStencilBuffer)}
    , size{size}
{
}

auto OffscreenSurface::getColorBuffer() const
    -> glow::Texture const &
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
    -> basix::Size<int>
{
    return size;
}

auto OffscreenSurface::setSize(basix::Size<int> const & newSize)
    -> void
{
    colorBuffer.setSize(newSize);

    depthAndStencilBuffer.setStorage(newSize);
}

} // namespace ape
