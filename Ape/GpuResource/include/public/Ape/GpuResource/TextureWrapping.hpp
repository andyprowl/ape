#pragma once

namespace ape
{

using GLint = int;

enum class TextureWrapping
{
    repeat,
    clampToEdge,
};

auto convertToOpenGLTextureWrapping(TextureWrapping wrapping)
    -> GLint;

} // namespace ape
