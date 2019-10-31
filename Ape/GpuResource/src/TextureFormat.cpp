#include <Ape/GpuResource/TextureFormat.hpp>

#include <glad/glad.h>

#include <cassert>
#include <unordered_map>

namespace ape
{

auto const textureFormatMap = std::unordered_map<TextureFormat, GLenum>{
    {TextureFormat::depthOnly, GL_DEPTH_COMPONENT},
    {TextureFormat::depthAndStencil, GL_DEPTH_STENCIL},
    {TextureFormat::redOnly, GL_RED},
    {TextureFormat::redGreenOnly, GL_RG},
    {TextureFormat::redGreenBlue, GL_RGB},
    {TextureFormat::redGreenBlueAlpha, GL_RGBA}};

auto convertToOpenGLFormat(TextureFormat const format)
    -> GLenum
{
    auto const it = textureFormatMap.find(format);

    if (it == std::cend(textureFormatMap))
    {
        assert(false);

        return 0;
    }

    return it->second;
}

auto convertFromOpenGLFormat(GLenum const format)
    -> TextureFormat
{
    for (auto const & entry : textureFormatMap)
    {
        if (entry.second == format)
        {
            return entry.first;
        }
    }

    assert(false);

    return TextureFormat::unknown;
}

} // namespace ape
