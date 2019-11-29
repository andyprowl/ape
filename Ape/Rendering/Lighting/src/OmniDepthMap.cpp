#include <Ape/Rendering/Lighting/OmniDepthMap.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>
#include <Glow/Texture/TextureStorageType.hpp>

#include <cassert>

namespace ape
{

namespace
{

auto makeOmniDepthMapFaceImage(basix::Size<int> const & size)
    -> glow::TextureImage
{
    return {nullptr, size, glow::TextureImageFormat::depth, glow::PixelType::floatingPoint};
}

auto makeOmniDepthMapImageSet(basix::Size<int> const & size)
    -> glow::CubeTextureImageSet
{
    return {
        makeOmniDepthMapFaceImage(size),
        makeOmniDepthMapFaceImage(size),
        makeOmniDepthMapFaceImage(size),
        makeOmniDepthMapFaceImage(size),
        makeOmniDepthMapFaceImage(size),
        makeOmniDepthMapFaceImage(size)};
}

auto makeOmniDepthMapTexture(basix::Size<int> const & size)
    -> glow::CubeTexture
{
    auto imageSet = makeOmniDepthMapImageSet(size);

    // TODO: Should we use depth32 or depth32f here?
    auto const descriptor = glow::CubeTextureDescriptor{
        std::move(imageSet),
        glow::TextureInternalFormat::depth32,
        glow::TextureWrapping::clampToEdge};

    return glow::CubeTexture{descriptor};
}

auto makeOmniDepthMapFrameBuffer(glow::CubeTexture & depthMapTexture)
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

OmniDepthMap::OmniDepthMap(basix::Size<int> const & size)
    : texture{makeOmniDepthMapTexture(size)}
    , frameBuffer{makeOmniDepthMapFrameBuffer(texture)}
    , size{size}
{
}

auto OmniDepthMap::getTexture()
    -> glow::CubeTexture &
{
    return texture;
}

auto OmniDepthMap::getTexture() const
    -> glow::CubeTexture const &
{
    return texture;
}

auto OmniDepthMap::getFrameBuffer()
    -> glow::FrameBufferObject &
{
    return frameBuffer;
}

auto OmniDepthMap::getFrameBuffer() const
    -> glow::FrameBufferObject const &
{
    return frameBuffer;
}

auto OmniDepthMap::getSize() const
    -> basix::Size<int>
{
    return size;
}

} // namespace ape
