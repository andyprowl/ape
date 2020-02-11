#pragma once

#include <Glow/Shader/UniformTypeTraits.hpp>

#include <glm/matrix.hpp>

#include <cstddef>
#include <string_view>
#include <type_traits>

namespace glow
{

class UniformBlock;

} // namespace glow

namespace glow::detail
{

class BasicUniformBlockMember
{

public:

    BasicUniformBlockMember(UniformBlock & block, std::string_view name);

    auto getBlock() const
        -> UniformBlock &;

    auto getIndex() const
        -> int;

    auto getOffset() const
        -> int;

private:

    auto fetchUniformIndex(std::string_view name) const
        -> int;

    auto fetchUniformOffset() const
        -> int;

private:

    UniformBlock * block;

    int index;

    int offset;

};

class BasicMatrixUniformBlockMember : public BasicUniformBlockMember
{

public:

    BasicMatrixUniformBlockMember(UniformBlock & block, std::string_view name);

    auto getStride() const
        -> int;

private:

    auto fetchMatrixStride() const
        -> int;

private:

    int stride;

};

} // namespace glow::detail

namespace glow
{

template<typename T>
class UniformBlockMember : public detail::BasicUniformBlockMember
{

public:

    using ValueType = T;

public:

    using BasicUniformBlockMember::BasicUniformBlockMember;

    auto get(const std::byte * buffer) const
        -> const ValueType &;

    auto set(ValueType const & value, std::byte * buffer)
        -> void;

};

template<glm::length_t N, glm::qualifier Q>
class UniformBlockMember<glm::mat<N, N, float, Q>> : public detail::BasicMatrixUniformBlockMember
{

public:

    using ValueType = glm::mat<N, N, float, Q>;

public:

    using BasicMatrixUniformBlockMember::BasicMatrixUniformBlockMember;

    auto get(const std::byte * buffer) const
        -> ValueType;

    auto set(ValueType const & value, std::byte * buffer)
        -> void;

};

template<typename MatrixType, std::enable_if_t<isMatrix<MatrixType>> * = nullptr>
auto getMatrix(const std::byte * const source, int const matrixStride)
    -> MatrixType
{
    constexpr auto const N = MatrixType::length();

    auto value = MatrixType{};

    for (auto col = 0; col < N; ++col)
    {
        auto const columnOffset = matrixStride * col;

        auto const columnSource = std::next(source, columnOffset);

        auto const columnTarget = reinterpret_cast<std::byte *>(&value[col]);

        std::copy(columnSource, std::next(columnSource, N * sizeof(float)), columnTarget);
    }

    return value;
}

template<typename MatrixType>
auto setMatrix(MatrixType const & value, std::byte * const target, int const matrixStride)
    -> void
{
    constexpr auto const N = MatrixType::length();

    for (auto col = 0; col < N; ++col)
    {
        auto const columnOffset = matrixStride * col;

        auto const columnTarget = std::next(target, columnOffset);

        auto const columnSource = reinterpret_cast<std::byte const *>(&value[col]);

        std::copy(columnSource, std::next(columnSource, N * sizeof(float)), columnTarget);
    }
}

} // namespace glow
