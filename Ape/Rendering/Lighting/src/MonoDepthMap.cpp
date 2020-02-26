#include <Ape/Rendering/Lighting/MonoDepthMap.hpp>

#include <glad/glad.h>

#include <cassert>

namespace ape
{

namespace
{

auto enableShadowSampling(glow::Texture2d & texture)
    -> void
{
    glTextureParameteri(texture.getId(), GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

    glTextureParameteri(texture.getId(), GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
}

auto makeDepthMapTexture(basix::Size2d<int> const & size, std::string_view const labelPrefix)
    -> glow::Texture2d
{
    auto image = glow::TextureImage{
        nullptr,
        size,
        glow::TextureImageFormat::depth,
        glow::PixelType::floatingPoint};

    // We do not want to allocate storage for additional mipmap levels for depth textures.
    auto numOfMipmapLevels = 1;

    // TODO: Should we use depth32 or depth32f here?
    auto const descriptor = glow::Texture2dDescriptor{
        std::move(image),
        glow::TextureInternalFormat::depth32,
        glow::TextureFiltering{
            glow::TextureMagnificationFilter::linear,
            glow::TextureMinificationFilter::linear},
        glow::TextureWrapping::clampToEdge,
        numOfMipmapLevels};

    auto texture = glow::Texture2d{descriptor, false, std::string{labelPrefix} + ".Texture2d"};

    enableShadowSampling(texture);

    return texture;
}

auto makeDepthMapFrameBuffer(glow::Texture2d & depthMapTexture, std::string_view const labelPrefix)
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

MonoDepthMap::MonoDepthMap(basix::Size2d<int> const & size)
    : MonoDepthMap{size, ""}
{
}

MonoDepthMap::MonoDepthMap(basix::Size2d<int> const & size, std::string_view const label)
    : texture{makeDepthMapTexture(size, label)}
    , frameBuffer{makeDepthMapFrameBuffer(texture, label)}
    , size{size}
{
}

auto MonoDepthMap::getTexture() const
    -> glow::Texture2d const &
{
    return texture;
}

auto MonoDepthMap::getFrameBuffer() const
    -> glow::FrameBufferObject const &
{
    return frameBuffer;
}

auto MonoDepthMap::getSize() const
    -> basix::Size2d<int>
{
    return size;
}

} // namespace ape
