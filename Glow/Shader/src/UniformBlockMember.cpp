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

    auto fetchedIndex = 0u;

    glGetUniformIndices(shaderId, 1, &nameData, &fetchedIndex);

    auto const uniformIndex = static_cast<int>(fetchedIndex);

    assert(uniformIndex >= 0);

    assert(glGetError() == GL_NO_ERROR);

    return uniformIndex;
}

auto BasicUniformBlockMember::fetchUniformOffset() const
    -> int
{
    auto const shaderId = block->getShader().getId();

    auto const uniformIndex = static_cast<unsigned int>(index);

    auto uniformOffset = 0;

    glGetActiveUniformsiv(shaderId, 1, &uniformIndex, GL_UNIFORM_OFFSET, &uniformOffset);

    assert(glGetError() == GL_NO_ERROR);

    return uniformOffset;
}

BasicMatrixUniformBlockMember::BasicMatrixUniformBlockMember(
    UniformBlock & block,
    std::string_view const name)
    : BasicUniformBlockMember{block, name}
    , stride{fetchMatrixStride()}
{
}

auto BasicMatrixUniformBlockMember::getStride() const
    -> int
{
    return stride;
}

auto BasicMatrixUniformBlockMember::fetchMatrixStride() const
    -> int
{
    auto const shaderId = getBlock().getShader().getId();

    auto const uniformIndex = static_cast<GLuint>(getIndex());

    auto matrixStride = 0;

    glGetActiveUniformsiv(shaderId, 1, &uniformIndex, GL_UNIFORM_MATRIX_STRIDE, &matrixStride);

    assert(glGetError() == GL_NO_ERROR);

    return matrixStride;
}

} // namespace glow::detail

namespace glow
{

template<typename T>
auto UniformBlockMember<T>::get(const std::byte * const buffer) const
    -> const T &
{
    auto const bufferOffset = getOffset();

    auto const data = reinterpret_cast<T const *>(std::next(buffer, bufferOffset));

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

template<glm::length_t N, glm::qualifier Q>
auto UniformBlockMember<glm::mat<N, N, float, Q>>::get(const std::byte * const buffer) const
    -> ValueType
{
    auto const bufferOffset = getOffset();

    auto const matrixStride = getStride();

    auto const source = std::next(buffer, bufferOffset);

    auto value = ValueType{};

    for (auto col = 0; col < N; ++col)
    {
        auto const columnOffset = matrixStride * col;

        auto const columnSource = std::next(source, columnOffset);

        auto const columnTarget = reinterpret_cast<std::byte *>(&value[col]);

        std::copy(columnSource, std::next(columnSource, N * sizeof(float)), columnTarget);
    }

    return value;
}

template<glm::length_t N, glm::qualifier Q>
auto UniformBlockMember<glm::mat<N, N, float, Q>>::set(
    ValueType const & value,
    std::byte * const buffer)
    -> void
{
    auto const bufferOffset = getOffset();

    auto const matrixStride = getStride();

    auto const target = std::next(buffer, bufferOffset);

    for (auto col = 0; col < N; ++col)
    {
        auto const columnOffset = matrixStride * col;

        auto const columnTarget = std::next(target, columnOffset);

        auto const columnSource = reinterpret_cast<std::byte const *>(&value[col]);

        std::copy(columnSource, std::next(columnSource, N * sizeof(float)), columnTarget);
    }
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

template class UniformBlockMember<glm::mat2>;

template class UniformBlockMember<glm::mat3>;

template class UniformBlockMember<glm::mat4>;

} // namespace glow
