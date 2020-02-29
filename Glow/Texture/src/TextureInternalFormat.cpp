#include <Glow/Texture/TextureInternalFormat.hpp>

#include <Glow/Texture/ColorSpace.hpp>
#include <Glow/Texture/TextureImageFormat.hpp>

#include <glad/glad.h>

#include <cassert>
#include <unordered_map>

namespace glow
{

namespace
{

auto const internalFormatMap = std::unordered_map<TextureInternalFormat, GLenum>{
    {TextureInternalFormat::r8, GL_R8},
    {TextureInternalFormat::rg8, GL_RG8},
    {TextureInternalFormat::rgb8, GL_RGB8},
    {TextureInternalFormat::rgba8, GL_RGBA8},
    {TextureInternalFormat::rgb16f, GL_RGB16F},
    {TextureInternalFormat::rgba16f, GL_RGBA16F},
    {TextureInternalFormat::srgb8, GL_SRGB8},
    {TextureInternalFormat::srgba8, GL_SRGB8_ALPHA8},
    {TextureInternalFormat::depth16, GL_DEPTH_COMPONENT16},
    {TextureInternalFormat::depth24, GL_DEPTH_COMPONENT24},
    {TextureInternalFormat::depth32, GL_DEPTH_COMPONENT32},
    {TextureInternalFormat::depth32f, GL_DEPTH_COMPONENT32F},
    {TextureInternalFormat::depth24Stencil8, GL_DEPTH24_STENCIL8},
    {TextureInternalFormat::depth32fStencil8, GL_DEPTH32F_STENCIL8},
    {TextureInternalFormat::compressedRgbaBptcUNorm, GL_COMPRESSED_RGBA_BPTC_UNORM},
    {TextureInternalFormat::compressedSrgbaBptcUNorm, GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM},
    {TextureInternalFormat::compressedRgbBptcSignedFloat, GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT},
    {TextureInternalFormat::compressedRgbBptcUnsignedFloat, GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT}};

} // unnamed namespace

auto convertToOpenGLInternalFormat(TextureInternalFormat const format)
    -> GLenum
{
    auto const it = internalFormatMap.find(format);

    if (it == std::cend(internalFormatMap))
    {
        assert(false);

        return 0;
    }

    return it->second;
}

auto convertFromOpenGLInternalFormat(GLenum const format)
    -> TextureInternalFormat
{
    for (auto const & entry : internalFormatMap)
    {
        if (entry.second == format)
        {
            return entry.first;
        }
    }

    assert(false);

    return TextureInternalFormat::unknown;
}

} // namespace glow
