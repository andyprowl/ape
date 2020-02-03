#pragma once

#include <string_view>

namespace glow
{

class ShaderProgram;
class UniformBufferObject;

class UniformBlock
{

public:

    UniformBlock(ShaderProgram & shader, std::string_view name);

    auto getShader() const
        -> ShaderProgram &;

    auto getIndex() const
        -> int;

    auto getSize() const
        -> int;

    auto getBindingPoint() const
        -> int;

    auto setBindingPoint(int newBindingPoint)
        -> void;

private:

    auto fetchIndexInProgram(std::string_view name) const
        -> int;

    auto fetchSize() const
        -> int;

private:

    ShaderProgram * shader;

    int index;

    int size;

    int bindingPoint;

};

auto setBlockDataSource(UniformBufferObject & buffer, UniformBlock const & block)
    -> void;

} // namespace glow
