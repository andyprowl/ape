#pragma once

#include <string_view>

namespace glow
{

class UniformBlock;

class UniformBlockMember
{

public:

    UniformBlockMember(UniformBlock & block, std::string_view name);

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

} // namespace glow
