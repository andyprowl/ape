#include <Glow/Texture/TextureFiltering.hpp>

#include <glad/glad.h>

#include <cassert>
#include <unordered_map>

namespace glow
{

namespace
{

auto const magnificationFilterMap = std::unordered_map<TextureMagnificationFilter, GLint>{
    {TextureMagnificationFilter::linear, GL_LINEAR},
    {TextureMagnificationFilter::nearest, GL_NEAREST}};

auto const minificationFilterMap = std::unordered_map<TextureMinificationFilter, GLint>{
    {TextureMinificationFilter::linear, GL_LINEAR},
    {TextureMinificationFilter::linearMipmapLinear, GL_LINEAR_MIPMAP_LINEAR},
    {TextureMinificationFilter::linearMipmapNearest, GL_LINEAR_MIPMAP_NEAREST},
    {TextureMinificationFilter::nearest, GL_NEAREST},
    {TextureMinificationFilter::nearestMipmapLinear, GL_NEAREST_MIPMAP_LINEAR},
    {TextureMinificationFilter::nearestMipmapNearest, GL_NEAREST_MIPMAP_NEAREST}};

} // unnamed namespace

auto convertToOpenGLTextureMagnificationFilter(TextureMagnificationFilter const filtering)
    -> GLint
{
    auto const it = magnificationFilterMap.find(filtering);

    if (it == std::cend(magnificationFilterMap))
    {
        assert(false);

        return 0;
    }

    return it->second;
}

auto convertToOpenGLTextureMinificationFilter(TextureMinificationFilter const filtering)
    -> GLint
{
    auto const it = minificationFilterMap.find(filtering);

    if (it == std::cend(minificationFilterMap))
    {
        assert(false);

        return 0;
    }

    return it->second;
}

} // namespace glow
