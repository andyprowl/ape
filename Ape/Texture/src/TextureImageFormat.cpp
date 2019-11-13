#include <Ape/Texture/TextureImageFormat.hpp>

#include <glad/glad.h>

#include <cassert>
#include <unordered_map>

namespace ape
{

auto const imageFormatMap = std::unordered_map<TextureImageFormat, GLenum>{
    {TextureImageFormat::r, GL_RED},
    {TextureImageFormat::rgb, GL_RGB},
    {TextureImageFormat::rgba, GL_RGBA},
    {TextureImageFormat::depth, GL_DEPTH_COMPONENT},
    {TextureImageFormat::depthStencil, GL_DEPTH_STENCIL}};

auto convertToOpenGLImageFormat(TextureImageFormat const format)
    -> GLenum
{
    auto const it = imageFormatMap.find(format);

    if (it == std::cend(imageFormatMap))
    {
        assert(false);

        return 0;
    }

    return it->second;
}

auto convertFromOpenGLImageFormat(GLenum const format)
    -> TextureImageFormat
{
    for (auto const & entry : imageFormatMap)
    {
        if (entry.second == format)
        {
            return entry.first;
        }
    }

    assert(false);

    return TextureImageFormat::unknown;
}

} // namespace ape
