#include <Ape/Rendering/DepthMap.hpp>

#include <Ape/GpuResource/ScopedBinder.hpp>

#include <cassert>

namespace ape
{

namespace
{

auto makeDepthMapTexture(Size<int> const & size)
    -> Texture
{
    auto const descriptor = TextureDescriptor{
        size,
        TextureFormat::depthOnly,
        PixelType::floatingPoint,
        TextureWrapping::clampToEdge,
        nullptr};

    return Texture{descriptor};
}

auto makeDepthMapFrameBuffer(Texture & depthMapTexture)
    -> FrameBufferObject
{
    auto frameBuffer = FrameBufferObject{};

    auto const binder = bind(frameBuffer);

    frameBuffer.attach(depthMapTexture, FrameBufferAttachment::depth);

    frameBuffer.resetDrawTarget();

    frameBuffer.resetReadTarget();

    assert(frameBuffer.isComplete());

    return frameBuffer;
}

} // unnamed namespace

DepthMap::DepthMap(Size<int> const & size)
    : texture{makeDepthMapTexture(size)}
    , frameBuffer{makeDepthMapFrameBuffer(texture)}
    , size{size}
{
}

auto DepthMap::getTexture() const
    -> Texture const &
{
    return texture;
}

auto DepthMap::getFrameBuffer() const
    -> FrameBufferObject const &
{
    return frameBuffer;
}

auto DepthMap::getSize() const
    -> Size<int>
{
    return size;
}

} // namespace ape
