#include <Ape/Rendering/Lighting/MonoDepthMap.hpp>

#include <glad/glad.h>

#include <cassert>
#include <string>

namespace ape
{

namespace
{

auto enableShadowSampling(glow::Texture2dArray & texture)
    -> void
{
    glTextureParameteri(texture.getId(), GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

    glTextureParameteri(texture.getId(), GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
}

auto makeDepthMapTexture(
    basix::Size2d<int> const & size,
    int const numOfLayers,
    std::string_view const labelPrefix)
    -> glow::Texture2dArray
{
    // We do not want to allocate storage for additional mipmap levels for depth textures.
    auto numOfMipmapLevels = 1;

    // TODO: Should we use depth32 or depth32f here?
    auto const layerDescriptor = glow::Texture2dDescriptor{
        size,
        glow::TextureInternalFormat::depth32,
        glow::TextureFiltering{
            glow::TextureMagnificationFilter::linear,
            glow::TextureMinificationFilter::linear},
        glow::TextureWrapping::clampToEdge,
        numOfMipmapLevels};

    auto texture = glow::Texture2dArray{
        layerDescriptor,
        numOfLayers,
        std::string{labelPrefix} + ".Texture"};

    enableShadowSampling(texture);

    return texture;
}

auto makeDepthMapFrameBuffer(
    glow::Texture2dArray & depthMapTexture,
    int const layer,
    std::string_view const labelPrefix)
    -> glow::FrameBufferObject
{
    auto const label = std::string{labelPrefix} + ".Framebuffer." + std::to_string(layer);

    auto frameBuffer = glow::FrameBufferObject{label};

    frameBuffer.attach(depthMapTexture, layer, glow::FrameBufferAttachment::depth);

    frameBuffer.resetDrawTarget();

    frameBuffer.resetReadTarget();

    assert(frameBuffer.isComplete());

    return frameBuffer;
}

auto makeDepthMapFrameBuffers(
    glow::Texture2dArray & depthMapTexture,
    int const numOfLayers,
    std::string_view const labelPrefix)
    -> std::vector<glow::FrameBufferObject>
{
    auto frameBuffers = std::vector<glow::FrameBufferObject>{};

    frameBuffers.reserve(numOfLayers);

    for (auto i = 0; i < numOfLayers; ++i)
    {
        auto framebuffer = makeDepthMapFrameBuffer(depthMapTexture, i, labelPrefix);

        frameBuffers.push_back(std::move(framebuffer));
    }

    return frameBuffers;
}

} // unnamed namespace

MonoDepthMap::MonoDepthMap(
    basix::Size2d<int> const & size,
    int const numOfLayers,
    std::string_view const label)
    : texture{makeDepthMapTexture(size, numOfLayers, label)}
    , frameBuffers{makeDepthMapFrameBuffers(texture, numOfLayers, label)}
    , size{size}
{
}

auto MonoDepthMap::getTexture() const
    -> glow::Texture2dArray const &
{
    return texture;
}

auto MonoDepthMap::getFrameBuffers() const
    -> std::vector<glow::FrameBufferObject> const &
{
    return frameBuffers;
}

auto MonoDepthMap::getSize() const
    -> basix::Size2d<int>
{
    return size;
}

} // namespace ape
