#pragma once

#include <glm/matrix.hpp>

#include <cstddef>
#include <string_view>

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

    auto get(const std::byte * buffer)
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

    auto get(const std::byte * buffer)
        -> ValueType;

    auto set(ValueType const & value, std::byte * buffer)
        -> void;

};

} // namespace glow
