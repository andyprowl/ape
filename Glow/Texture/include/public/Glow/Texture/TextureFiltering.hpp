#pragma once

namespace glow
{

using GLint = int;

enum class TextureMagnificationFilter
{
    linear,
    nearest
};

enum class TextureMinificationFilter
{
    linear,
    linearMipmapLinear,
    linearMipmapNearest,
    nearest,
    nearestMipmapLinear,
    nearestMipmapNearest
};

class TextureFiltering
{

public:

    TextureFiltering(
        TextureMagnificationFilter const magnification,
        TextureMinificationFilter const minification)
        : magnification{magnification}
        , minification{minification}
    {
    }

public:

    TextureMagnificationFilter magnification;

    TextureMinificationFilter minification;

};

auto convertToOpenGLTextureMagnificationFilter(TextureMagnificationFilter filter)
    -> GLint;

auto convertToOpenGLTextureMinificationFilter(TextureMinificationFilter filter)
    -> GLint;

} // namespace glow
