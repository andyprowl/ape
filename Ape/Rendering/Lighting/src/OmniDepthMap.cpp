#include <Ape/Rendering/Lighting/OmniDepthMap.hpp>

#include <glad/glad.h>

#include <cassert>

namespace ape
{

namespace
{

auto enableShadowSampling(glow::TextureCubeArray & texture)
    -> void
{
    glTextureParameteri(texture.getId(), GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

    glTextureParameteri(texture.getId(), GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
}

auto makeOmniDepthMapTexture(
    basix::Size2d<int> const & size,
    int const numOfLayers,
    std::string_view const labelPrefix)
    -> glow::TextureCubeArray
{
    // We do not want to allocate storage for additional mipmap levels for depth textures.
    auto numOfMipmapLevels = 1;

    // TODO: Should we use depth32 or depth32f here?
    auto const faceDescriptor = glow::Texture2dDescriptor{
        size,
        glow::TextureInternalFormat::depth32,
        glow::TextureFiltering{
            glow::TextureMagnificationFilter::linear,
            glow::TextureMinificationFilter::linear},
        glow::TextureWrapping::clampToEdge,
        numOfMipmapLevels};

    auto texture = glow::TextureCubeArray{
        faceDescriptor,
        numOfLayers,
        std::string{labelPrefix} + ".Texture"};

    enableShadowSampling(texture);

    return texture;
}

auto makeDepthMapFrameBuffer(
    glow::TextureCubeArray & depthMapTexture,
    int const layer,
    int const faceIndex,
    std::string_view const labelPrefix)
    -> glow::FrameBufferObject
{
    auto frameBuffer = glow::FrameBufferObject{std::string{labelPrefix} + ".Framebuffer"};

    auto const face = static_cast<glow::TextureCubeFace>(faceIndex);

    frameBuffer.attach(depthMapTexture, layer, face, glow::FrameBufferAttachment::depth);

    frameBuffer.resetDrawTarget();

    frameBuffer.resetReadTarget();

    assert(frameBuffer.isComplete());

    return frameBuffer;
}

auto makeDepthMapFrameBuffers(
    glow::TextureCubeArray & depthMapTexture,
    int const numOfLayers,
    std::string_view const labelPrefix)
    -> std::vector<glow::FrameBufferObject>
{
    auto frameBuffers = std::vector<glow::FrameBufferObject>{};

    frameBuffers.reserve(numOfLayers);

    for (auto i = 0; i < numOfLayers; ++i)
    {
        for (auto face = 0; face < 6; ++face)
        {
            auto framebuffer = makeDepthMapFrameBuffer(depthMapTexture, i, face, labelPrefix);

            frameBuffers.push_back(std::move(framebuffer));
        }
    }

    return frameBuffers;
}

} // unnamed namespace

OmniDepthMap::OmniDepthMap(
    basix::Size2d<int> const & size, 
    int const numOfLayers,
    std::string_view const label)
    : texture{makeOmniDepthMapTexture(size, numOfLayers, label)}
    , frameBuffers{makeDepthMapFrameBuffers(texture, numOfLayers, label)}
    , size{size}
{
}

auto OmniDepthMap::getTexture() const
    -> glow::TextureCubeArray const &
{
    return texture;
}

auto OmniDepthMap::getFrameBuffers() const
    -> std::vector<glow::FrameBufferObject> const &
{
    return frameBuffers;
}

auto OmniDepthMap::getFrameBuffer(int const layer, glow::TextureCubeFace const face) const
    -> glow::FrameBufferObject const &
{
    auto const faceIndex = static_cast<int>(face);

    auto const layerFace = layer * 6 + faceIndex;

    return frameBuffers[layerFace];
}

auto OmniDepthMap::getSize() const
    -> basix::Size2d<int>
{
    return size;
}

} // namespace ape
