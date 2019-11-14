#include <Glow/Texture/TextureSwizzleMask.hpp>

namespace glow
{

auto convertToOpenGLSwizzleMask(TextureSwizzleMask const & mask)
    -> std::array<GLint, 4u>
{
    return {{
        convertToOpenGLSwizzle(mask.red),
        convertToOpenGLSwizzle(mask.green),
        convertToOpenGLSwizzle(mask.blue),
        convertToOpenGLSwizzle(mask.alpha)}};
}

} // namespace glow
