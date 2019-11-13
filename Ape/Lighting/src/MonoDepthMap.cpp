#include <Ape/Lighting/MonoDepthMap.hpp>

#include <Ape/GpuResource/ScopedBinder.hpp>
#include <Ape/Texture/TextureStorageType.hpp>

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

    // TODO: Should we use depth32 or depth32f here?
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

MonoDepthMap::MonoDepthMap(Size<int> const & size)
    : texture{makeDepthMapTexture(size)}
    , frameBuffer{makeDepthMapFrameBuffer(texture)}
    , size{size}
{
}

auto MonoDepthMap::getTexture() const
    -> Texture const &
{
    return texture;
}

auto MonoDepthMap::getFrameBuffer() const
    -> FrameBufferObject const &
{
    return frameBuffer;
}

auto MonoDepthMap::getSize() const
    -> Size<int>
{
    return size;
}

} // namespace ape
