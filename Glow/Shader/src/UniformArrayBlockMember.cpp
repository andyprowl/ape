#include <Glow/Shader/UniformArrayBlockMember.hpp>

#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/UniformBlock.hpp>
#include <Glow/Shader/Uniform.hpp>

#include <glad/glad.h>

namespace glow::detail
{

BasicUniformArrayBlockMember::BasicUniformArrayBlockMember(
    UniformBlock & block,
    std::string_view const name)
    : BasicUniformBlockMember{block, name}
    , arrayStride{fetchArrayStride()}
{
}

auto BasicUniformArrayBlockMember::getArrayStride() const
    -> int
{
    return arrayStride;
}

auto BasicUniformArrayBlockMember::fetchArrayStride() const
    -> int
{
    auto const shaderId = getBlock().getShader().getId();

    auto const uniformIndex = static_cast<GLuint>(getIndex());

    auto stride = 0;

    glGetActiveUniformsiv(shaderId, 1, &uniformIndex, GL_UNIFORM_ARRAY_STRIDE, &stride);

    assert(glGetError() == GL_NO_ERROR);

    return stride;
}

BasicUniformMatrixArrayBlockMember::BasicUniformMatrixArrayBlockMember(
    UniformBlock & block,
    std::string_view const name)
    : BasicUniformArrayBlockMember{block, name}
    , matrixStride{fetchMatrixStride(name)}
{
}

auto BasicUniformMatrixArrayBlockMember::getMatrixStride() const
    -> int
{
    return matrixStride;
}

auto BasicUniformMatrixArrayBlockMember::fetchMatrixStride(std::string_view const name) const
    -> int
{
    auto const & shader = getBlock().getShader();

    auto uniformIndex = static_cast<GLuint>(fetchFirstElementUniformIndex(name));

    auto stride = 0;

    glGetActiveUniformsiv(shader.getId(), 1, &uniformIndex, GL_UNIFORM_MATRIX_STRIDE, &stride);

    assert(glGetError() == GL_NO_ERROR);

    return stride;
}

auto BasicUniformMatrixArrayBlockMember::fetchFirstElementUniformIndex(std::string_view name) const
    -> int
{
    auto const & shader = getBlock().getShader();

    auto const firstElementName = std::string{name} + "[0]";

    return getUniformIndex(shader, firstElementName);
}

} // namespace glow::detail
