#include <Ape/Rendering/OffscreenSurface.hpp>

#include <Ape/GpuResource/ScopedBinder.hpp>

#include <cassert>

namespace ape
{

namespace
{

auto makeColorBuffer(Size<int> const & size)
    -> Texture
{
    auto const descriptor = TextureDescriptor{
        size,
        TextureFormat::redGreenBlue,
        PixelType::unsignedByte,
        TextureWrapping::clampToEdge,
        nullptr};

    return Texture{descriptor};
}

auto makeDepthAndStencilBuffer(Size<int> const & size)
    -> RenderBufferObject
{
    return RenderBufferObject{size};
}

auto makeFrameBufferForTarget(Texture & colorBuffer, RenderBufferObject & depthAndStencilBuffer)
    -> FrameBufferObject
{
    auto frameBuffer = FrameBufferObject{};

    auto const binder = bind(frameBuffer);

    frameBuffer.attach(colorBuffer, FrameBufferAttachment::color0);
    
    frameBuffer.attach(depthAndStencilBuffer, FrameBufferAttachment::depthAndStencil);

    assert(frameBuffer.isComplete());

    return frameBuffer;
}

} // unnamed namespace

OffscreenSurface::OffscreenSurface(Size<int> const & size)
    : colorBuffer{makeColorBuffer(size)}
    , depthAndStencilBuffer{makeDepthAndStencilBuffer(size)}
    , frameBuffer{makeFrameBufferForTarget(colorBuffer, depthAndStencilBuffer)}
    , size{size}
{
}

auto OffscreenSurface::getColorBuffer() const
    -> Texture const &
{
    return colorBuffer;
}

auto OffscreenSurface::getDepthAndStencilBuffer() const
    -> RenderBufferObject const &
{
    return depthAndStencilBuffer;
}

auto OffscreenSurface::getFrameBuffer() const
    -> FrameBufferObject const &
{
    return frameBuffer;
}

auto OffscreenSurface::getSize() const
    -> Size<int>
{
    return size;
}

auto OffscreenSurface::setSize(Size<int> const & newSize)
    -> void
{
    colorBuffer.setSize(newSize);

    depthAndStencilBuffer.setStorage(newSize);
}

} // namespace ape
