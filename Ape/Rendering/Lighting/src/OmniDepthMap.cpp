#include <Ape/Rendering/Lighting/OmniDepthMap.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>
#include <Glow/Texture/TextureStorageType.hpp>

#include <glad/glad.h>

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

auto enableShadowSampling(glow::CubeTexture & texture)
    -> void
{
    auto const binder = glow::bind(texture);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
}

auto makeOmniDepthMapTexture(
    basix::Size<int> const & size,
    std::string_view const labelPrefix)
    -> glow::CubeTexture
{
    auto imageSet = makeOmniDepthMapImageSet(size);

    // TODO: Should we use depth32 or depth32f here?
    auto const descriptor = glow::CubeTextureDescriptor{
        std::move(imageSet),
        glow::TextureInternalFormat::depth32,
        glow::TextureWrapping::clampToEdge};

    auto texture = glow::CubeTexture{descriptor, std::string{labelPrefix} + ".Texture"};

    enableShadowSampling(texture);

    return texture;
}

auto makeOmniDepthMapFrameBuffer(
    glow::CubeTexture & depthMapTexture,
    std::string_view const labelPrefix)
    -> glow::FrameBufferObject
{
    auto frameBuffer = glow::FrameBufferObject{std::string{labelPrefix} + ".Framebuffer"};

    auto const binder = glow::bind(frameBuffer);

    frameBuffer.attach(depthMapTexture, glow::FrameBufferAttachment::depth);

    frameBuffer.resetDrawTarget();

    frameBuffer.resetReadTarget();

    assert(frameBuffer.isComplete());

    return frameBuffer;
}

} // unnamed namespace

OmniDepthMap::OmniDepthMap(basix::Size<int> const & size)
    : OmniDepthMap{size, ""}
{
}

OmniDepthMap::OmniDepthMap(basix::Size<int> const & size, std::string_view const label)
    : texture{makeOmniDepthMapTexture(size, label)}
    , frameBuffer{makeOmniDepthMapFrameBuffer(texture, label)}
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
