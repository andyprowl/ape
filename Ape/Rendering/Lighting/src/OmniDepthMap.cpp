#include <Ape/Rendering/Lighting/OmniDepthMap.hpp>

#include <glad/glad.h>

#include <cassert>

namespace ape
{

namespace
{

auto enableShadowSampling(glow::TextureCube & texture)
    -> void
{
    glTextureParameteri(texture.getId(), GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

    glTextureParameteri(texture.getId(), GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
}

auto makeOmniDepthMapTexture(
    basix::Size2d<int> const & size,
    std::string_view const labelPrefix)
    -> glow::TextureCube
{
    // We do not want to allocate storage for additional mipmap levels for depth textures.
    auto numOfMipmapLevels = 1;

    // TODO: Should we use depth32 or depth32f here?
    auto const faceeDescriptor = glow::Texture2dDescriptor{
        size,
        glow::TextureInternalFormat::depth32,
        glow::TextureFiltering{
            glow::TextureMagnificationFilter::linear,
            glow::TextureMinificationFilter::linear},
        glow::TextureWrapping::clampToEdge,
        numOfMipmapLevels};

    auto texture = glow::TextureCube{faceeDescriptor, std::string{labelPrefix} + ".Texture2d"};

    enableShadowSampling(texture);

    return texture;
}

auto makeOmniDepthMapFrameBuffer(
    glow::TextureCube & depthMapTexture,
    std::string_view const labelPrefix)
    -> glow::FrameBufferObject
{
    auto frameBuffer = glow::FrameBufferObject{std::string{labelPrefix} + ".Framebuffer"};

    frameBuffer.attach(depthMapTexture, glow::FrameBufferAttachment::depth);

    frameBuffer.resetDrawTarget();

    frameBuffer.resetReadTarget();

    assert(frameBuffer.isComplete());

    return frameBuffer;
}

} // unnamed namespace

OmniDepthMap::OmniDepthMap(basix::Size2d<int> const & size)
    : OmniDepthMap{size, ""}
{
}

OmniDepthMap::OmniDepthMap(basix::Size2d<int> const & size, std::string_view const label)
    : texture{makeOmniDepthMapTexture(size, label)}
    , frameBuffer{makeOmniDepthMapFrameBuffer(texture, label)}
    , size{size}
{
}

auto OmniDepthMap::getTexture()
    -> glow::TextureCube &
{
    return texture;
}

auto OmniDepthMap::getTexture() const
    -> glow::TextureCube const &
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
    -> basix::Size2d<int>
{
    return size;
}

} // namespace ape
