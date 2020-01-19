#include <Glow/Texture/TextureWrapping.hpp>

#include <glad/glad.h>

#include <array>
#include <cassert>

namespace glow
{

namespace
{

constexpr auto swizzleCodes = std::array<GLint, 2u>{
    GL_REPEAT,
    GL_CLAMP_TO_EDGE};

} // unnamed namespace

auto convertToOpenGLTextureWrapping(TextureWrapping const wrapping)
    -> GLint
{
    return swizzleCodes[static_cast<std::size_t>(wrapping)];
}

} // namespace glow
