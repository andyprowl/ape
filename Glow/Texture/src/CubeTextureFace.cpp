#include <Glow/Texture/TextureCubeFace.hpp>

#include <glad/glad.h>

namespace glow
{

auto convertToOpenGLFace(TextureCubeFace const face)
    -> GLenum
{
    return GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<int>(face);
}

} // namespace glow
