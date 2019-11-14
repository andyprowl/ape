#include <Glow/Texture/TextureSwizzle.hpp>

#include <glad/glad.h>

#include <array>

namespace glow
{

namespace
{

constexpr auto swizzleCodes = std::array<GLint, 6u>{
    GL_RED,
    GL_GREEN,
    GL_BLUE,
    GL_ALPHA,
    GL_ZERO,
    GL_ONE};

} // unnamed namespace

auto convertToOpenGLSwizzle(const TextureSwizzle swizzle)
    -> GLint
{
    return swizzleCodes[static_cast<std::size_t>(swizzle)];
}

} // namespace glow
