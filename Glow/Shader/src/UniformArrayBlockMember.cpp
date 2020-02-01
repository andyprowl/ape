#include <Glow/Shader/UniformArrayBlockMember.hpp>

#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/UniformBlock.hpp>

#include <glad/glad.h>

namespace glow::detail
{

BasicUniformArrayBlockMember::BasicUniformArrayBlockMember(
    UniformBlock & block,
    std::string_view const name)
    : BasicUniformBlockMember{block, name}
    , stride{fetchArrayStride()}
    , maxNumOfElements{fetchMaxNumOfElements()}
{
}

auto BasicUniformArrayBlockMember::getStride() const
    -> int
{
    return stride;
}

auto BasicUniformArrayBlockMember::getMaxNumOfElements() const
    -> int
{
    return maxNumOfElements;
}

auto BasicUniformArrayBlockMember::fetchArrayStride() const
    -> int
{
    auto const shaderId = getBlock().getShader().getId();

    auto const uniformIndex = static_cast<GLuint>(getIndex());

    auto arrayStride = 0;

    glGetActiveUniformsiv(shaderId, 1, &uniformIndex, GL_UNIFORM_ARRAY_STRIDE, &arrayStride);

    assert(glGetError() == GL_NO_ERROR);

    return arrayStride;
}

auto BasicUniformArrayBlockMember::fetchMaxNumOfElements() const
    -> int
{
    auto const shaderId = getBlock().getShader().getId();

    auto const uniformIndex = static_cast<GLuint>(getIndex());

    auto arraySize = 0;

    glGetActiveUniformsiv(shaderId, 1, &uniformIndex, GL_UNIFORM_SIZE, &arraySize);

    assert(glGetError() == GL_NO_ERROR);

    return arraySize;
}

} // namespace glow::detail
