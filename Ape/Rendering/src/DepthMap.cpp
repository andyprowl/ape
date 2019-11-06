#include <Ape/Rendering/DepthMap.hpp>

#include <Ape/GpuResource/ScopedBinder.hpp>
#include <Ape/GpuResource/TextureStorageType.hpp>

#include <cassert>

namespace ape
{

namespace
{

auto makeDepthMapTexture(Size<int> const & size)
    -> Texture
{
    auto const image = TextureImage{
        nullptr,
        size,
        TextureImageFormat::depth,
        PixelType::floatingPoint};

    auto const descriptor = TextureDescriptor{
        image,
        TextureInternalFormat::depth32,
        TextureWrapping::clampToEdge,
        TextureStorageType::immutable};

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
