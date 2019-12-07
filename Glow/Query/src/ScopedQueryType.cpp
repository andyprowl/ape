#include <Glow/Query/ScopedQueryType.hpp>

#include <glad/glad.h>

#include <array>

namespace glow
{

namespace
{

constexpr auto queryTypes = std::array<GLenum, 6u>{
    GL_SAMPLES_PASSED,
    GL_ANY_SAMPLES_PASSED,
    GL_ANY_SAMPLES_PASSED_CONSERVATIVE,
    GL_PRIMITIVES_GENERATED,
    GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN,
    GL_TIME_ELAPSED};

} // unnamed namespace

auto convertToOpenGLQueryType(ScopedQueryType const type)
    -> GLenum
{
    return queryTypes[static_cast<std::size_t>(type)];
}

} // namespace glow
