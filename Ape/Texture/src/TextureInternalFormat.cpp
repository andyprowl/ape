#include <Ape/Texture/TextureInternalFormat.hpp>

#include <Ape/Texture/ColorSpace.hpp>
#include <Ape/Texture/TextureImageFormat.hpp>

#include <glad/glad.h>

#include <cassert>
#include <unordered_map>

namespace ape
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
    {TextureInternalFormat::depth32fStencil8, GL_DEPTH32F_STENCIL8}};

auto const linearSpaceFormatMap = std::unordered_map<TextureImageFormat, TextureInternalFormat>{
    {TextureImageFormat::r, TextureInternalFormat::r8},
    {TextureImageFormat::rgb, TextureInternalFormat::rgb8},
    {TextureImageFormat::rgba, TextureInternalFormat::rgba8}};

auto const gammaSpaceFormatMap = std::unordered_map<TextureImageFormat, TextureInternalFormat>{
    {TextureImageFormat::r, TextureInternalFormat::r8},
    {TextureImageFormat::rgb, TextureInternalFormat::srgb8},
    {TextureImageFormat::rgba, TextureInternalFormat::srgba8}};

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

auto determineInternalFormat(TextureImageFormat const format, ColorSpace const imageColorSpace)
    -> TextureInternalFormat
{
    if (imageColorSpace == ColorSpace::linear)
    {
        return linearSpaceFormatMap.at(format);
    }
    else
    {
        assert(imageColorSpace == ColorSpace::perceptual);

        return gammaSpaceFormatMap.at(format);
    }
}

} // namespace ape
