#include <Ape/Lighting/MonoDepthMap.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>
#include <Glow/Texture/TextureStorageType.hpp>

#include <cassert>

namespace ape
{

namespace
{

auto makeDepthMapTexture(basix::Size<int> const & size)
    -> glow::Texture
{
    auto const image = glow::TextureImage{
        nullptr,
        size,
        glow::TextureImageFormat::depth,
        glow::PixelType::floatingPoint};

    // TODO: Should we use depth32 or depth32f here?
    auto const descriptor = glow::TextureDescriptor{
        image,
        glow::TextureInternalFormat::depth32,
        glow::TextureWrapping::clampToEdge,
        glow::TextureStorageType::immutable};

    return glow::Texture{descriptor};
}

auto makeDepthMapFrameBuffer(glow::Texture & depthMapTexture)
    -> glow::FrameBufferObject
{
    auto frameBuffer = glow::FrameBufferObject{};

    auto const binder = glow::bind(frameBuffer);

    frameBuffer.attach(depthMapTexture, glow::FrameBufferAttachment::depth);

    frameBuffer.resetDrawTarget();

    frameBuffer.resetReadTarget();

    assert(frameBuffer.isComplete());

    return frameBuffer;
}

} // unnamed namespace

MonoDepthMap::MonoDepthMap(basix::Size<int> const & size)
    : texture{makeDepthMapTexture(size)}
    , frameBuffer{makeDepthMapFrameBuffer(texture)}
    , size{size}
{
}

auto MonoDepthMap::getTexture() const
    -> glow::Texture const &
{
    return texture;
}

auto MonoDepthMap::getFrameBuffer() const
    -> glow::FrameBufferObject const &
{
    return frameBuffer;
}

auto MonoDepthMap::getSize() const
    -> basix::Size<int>
{
    return size;
}

} // namespace ape
