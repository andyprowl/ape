#include <Glow/Shader/UniformBlockMember.hpp>

#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/UniformBlock.hpp>

#include <glad/glad.h>

#include <cassert>

namespace glow
{

UniformBlockMember::UniformBlockMember(UniformBlock & block, std::string_view const name)
    : block{&block}
    , index{fetchUniformIndex(name)}
    , offset{fetchUniformOffset()}
{
}

auto UniformBlockMember::getBlock() const
    -> UniformBlock &
{
    return *block;
}

auto UniformBlockMember::getIndex() const
    -> int
{
    return index;
}

auto UniformBlockMember::getOffset() const
    -> int
{
    return offset;
}

auto UniformBlockMember::fetchUniformIndex(std::string_view const name) const
    -> int
{
    auto const shaderId = block->getShader().getId();

    auto const nameData = name.data();

    auto uniformIndex = 0u;

    glGetUniformIndices(shaderId, 1, &nameData, &uniformIndex);

    assert(glGetError() == GL_NO_ERROR);

    return static_cast<int>(uniformIndex);
}

auto UniformBlockMember::fetchUniformOffset() const
    -> int
{
    auto const shaderId = block->getShader().getId();

    auto const uniformIndex = static_cast<unsigned int>(index);

    auto uniformOffset = 0;

    glGetActiveUniformsiv(shaderId, 1, &uniformIndex, GL_UNIFORM_OFFSET, &uniformOffset);

    return uniformOffset;
}

} // namespace glow
