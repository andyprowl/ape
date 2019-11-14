#pragma once

namespace glow
{

using GLenum = unsigned int;

enum class TextureImageFormat
{
    unknown,
    r,
    rgb,
    rgba,
    depth,
    depthStencil,
};

auto convertToOpenGLImageFormat(TextureImageFormat format)
    -> GLenum;

auto convertFromOpenGLImageFormat(GLenum format)
    -> TextureImageFormat;

} // namespace glow
