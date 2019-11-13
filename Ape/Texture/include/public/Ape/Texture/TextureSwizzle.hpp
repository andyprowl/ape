#pragma once

namespace ape
{

using GLint = int;

enum class TextureSwizzle
{
    red,
    green,
    blue,
    alpha,
    zero,
    one
};

auto convertToOpenGLSwizzle(TextureSwizzle swizzle)
    -> GLint;

} // namespace ape
