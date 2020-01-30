#include <Glow/Shader/UniformBlockMember.hpp>

#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/UniformBlock.hpp>

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <algorithm>
#include <cassert>

namespace glow::detail
{

BasicUniformBlockMember::BasicUniformBlockMember(UniformBlock & block, std::string_view const name)
    : block{&block}
    , index{fetchUniformIndex(name)}
    , offset{fetchUniformOffset()}
{
}

auto BasicUniformBlockMember::getBlock() const
    -> UniformBlock &
{
    return *block;
}

auto BasicUniformBlockMember::getIndex() const
    -> int
{
    return index;
}

auto BasicUniformBlockMember::getOffset() const
    -> int
{
    return offset;
}

auto BasicUniformBlockMember::fetchUniformIndex(std::string_view const name) const
    -> int
{
    auto const shaderId = block->getShader().getId();

    auto const nameData = name.data();

    auto uniformIndex = 0u;

    glGetUniformIndices(shaderId, 1, &nameData, &uniformIndex);

    assert(glGetError() == GL_NO_ERROR);

    return static_cast<int>(uniformIndex);
}

auto BasicUniformBlockMember::fetchUniformOffset() const
    -> int
{
    auto const shaderId = block->getShader().getId();

    auto const uniformIndex = static_cast<unsigned int>(index);

    auto uniformOffset = 0;

    glGetActiveUniformsiv(shaderId, 1, &uniformIndex, GL_UNIFORM_OFFSET, &uniformOffset);

    return uniformOffset;
}

} // namespace glow::detail

namespace glow
{

template<typename T>
auto UniformBlockMember<T>::get(const std::byte * const buffer)
    -> const T &
{
    auto const bufferOffset = getOffset();

    auto const data = reinterpret_cast<T const *>(buffer + bufferOffset);

    return *data;
}

template<typename T>
auto UniformBlockMember<T>::set(T const & value, std::byte * const buffer)
    -> void
{
    auto const bufferOffset = getOffset();

    auto const target = std::next(buffer, bufferOffset);

    auto const source = reinterpret_cast<std::byte const *>(&value);

    std::copy(source, std::next(source, sizeof(value)), target);
}

template class UniformBlockMember<bool>;

template class UniformBlockMember<float>;

template class UniformBlockMember<int>;

template class UniformBlockMember<glm::vec2>;

template class UniformBlockMember<glm::ivec2>;

template class UniformBlockMember<glm::vec3>;

template class UniformBlockMember<glm::ivec3>;

template class UniformBlockMember<glm::vec4>;

template class UniformBlockMember<glm::ivec4>;

} // namespace glow
