#pragma once

namespace ape
{

using GLenum = unsigned int;

enum class TextureImageFormat
{
    unknown,
    rgb,
    rgba,
    depth,
    depthStencil,
};

auto convertToOpenGLImageFormat(TextureImageFormat format)
    -> GLenum;

auto convertFromOpenGLImageFormat(GLenum format)
    -> TextureImageFormat;

} // namespace ape
