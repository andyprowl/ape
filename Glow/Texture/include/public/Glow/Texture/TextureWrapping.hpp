#pragma once

namespace glow
{

using GLint = int;

enum class TextureWrapping
{
    repeat,
    clampToEdge,
};

auto convertToOpenGLTextureWrapping(TextureWrapping wrapping)
    -> GLint;

} // namespace glow
