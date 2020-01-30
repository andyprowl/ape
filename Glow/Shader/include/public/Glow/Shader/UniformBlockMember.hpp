#pragma once

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

} // namespace glow::detail

namespace glow
{

template<typename T>
class UniformBlockMember : public detail::BasicUniformBlockMember
{

public:

    using BasicUniformBlockMember::BasicUniformBlockMember;

    auto get(const std::byte * buffer)
        -> const T &;

    auto set(T const & value, std::byte * buffer)
        -> void;

};

} // namespace glow
