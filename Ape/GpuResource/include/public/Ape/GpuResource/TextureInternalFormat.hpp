#pragma once

namespace ape
{

using GLenum = unsigned int;

enum class TextureInternalFormat
{
    unknown,
    rgb8,
    rgba8,
    rgb16f,
    rgba16f,
    srgb8,
    srgba8,
    depth16,
    depth24,
    depth32,
    depth32f,
    depth24Stencil8,
    depth32fStencil8
};

auto convertToOpenGLInternalFormat(TextureInternalFormat format)
    -> GLenum;

auto convertFromOpenGLInternalFormat(GLenum format)
    -> TextureInternalFormat;

} // namespace ape
