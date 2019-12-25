#include <Ape/Rendering/Lighting/MonoDepthMap.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>
#include <Glow/Texture/TextureStorageType.hpp>

#include <glad/glad.h>

#include <cassert>

namespace ape
{

namespace
{

auto enableShadowSampling(glow::Texture & texture)
    -> void
{
    auto const binder = glow::bind(texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
}

auto makeDepthMapTexture(basix::Size<int> const & size, std::string_view const labelPrefix)
    -> glow::Texture
{
    auto image = glow::TextureImage{
        nullptr,
        size,
        glow::TextureImageFormat::depth,
        glow::PixelType::floatingPoint};

    // TODO: Should we use depth32 or depth32f here?
    auto const descriptor = glow::TextureDescriptor{
        std::move(image),
        glow::TextureInternalFormat::depth32,
        glow::TextureWrapping::clampToEdge,
        glow::TextureStorageType::immutable};

    auto texture = glow::Texture{descriptor, std::string{labelPrefix} + ".Texture"};

    enableShadowSampling(texture);

    return texture;
}

auto makeDepthMapFrameBuffer(glow::Texture & depthMapTexture, std::string_view const labelPrefix)
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

MonoDepthMap::MonoDepthMap(basix::Size<int> const & size)
    : MonoDepthMap{size, ""}
{
}

MonoDepthMap::MonoDepthMap(basix::Size<int> const & size, std::string_view const label)
    : texture{makeDepthMapTexture(size, label)}
    , frameBuffer{makeDepthMapFrameBuffer(texture, label)}
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
