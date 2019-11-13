#include <Ape/Lighting/OmniDepthMap.hpp>

#include <Ape/GpuResource/ScopedBinder.hpp>
#include <Ape/Texture/TextureStorageType.hpp>

#include <cassert>

namespace ape
{

namespace
{

auto makeOmniDepthMapFaceImage(Size<int> const & size)
    -> TextureImage
{
    return {nullptr, size, TextureImageFormat::depth, PixelType::floatingPoint};
}

auto makeOmniDepthMapImageSet(Size<int> const & size)
    -> CubeTextureImageSet
{
    auto const faceImage = makeOmniDepthMapFaceImage(size);

    return {faceImage, faceImage, faceImage, faceImage, faceImage, faceImage};
}

auto makeOmniDepthMapTexture(Size<int> const & size)
    -> CubeTexture
{
    auto const imageSet = makeOmniDepthMapImageSet(size);

    // TODO: Should we use depth32 or depth32f here?
    auto const descriptor = CubeTextureDescriptor{
        imageSet,
        TextureInternalFormat::depth32,
        TextureWrapping::clampToEdge};

    return CubeTexture{descriptor};
}

auto makeOmniDepthMapFrameBuffer(CubeTexture & depthMapTexture)
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

OmniDepthMap::OmniDepthMap(Size<int> const & size)
    : texture{makeOmniDepthMapTexture(size)}
    , frameBuffer{makeOmniDepthMapFrameBuffer(texture)}
    , size{size}
{
}

auto OmniDepthMap::getTexture() const
    -> CubeTexture const &
{
    return texture;
}

auto OmniDepthMap::getFrameBuffer() const
    -> FrameBufferObject const &
{
    return frameBuffer;
}

auto OmniDepthMap::getSize() const
    -> Size<int>
{
    return size;
}

} // namespace ape
