#include <Glow/Shader/UniformBlock.hpp>

#include <Glow/Shader/ShaderProgram.hpp>

#include <glad/glad.h>

#include <cassert>

namespace glow
{

UniformBlock::UniformBlock(ShaderProgram & shader, std::string_view const name)
    : shader{&shader}
    , index{fetchIndexInProgram(name)}
    , size{fetchSize()}
    , bindingPoint{-1}
{
}

auto UniformBlock::getShader() const
    -> ShaderProgram &
{
    return *shader;
}

auto UniformBlock::getIndex() const
    -> int
{
    return index;
}

auto UniformBlock::getSize() const
    -> int
{
    return size;
}

auto UniformBlock::getBindingPoint() const
    -> int
{
    return bindingPoint;
}

auto UniformBlock::setBindingPoint(int const newBindingPoint)
    -> void
{
    if (newBindingPoint == bindingPoint)
    {
        return;
    }

    bindingPoint = newBindingPoint;

    auto const shaderId = shader->getId();

    glUniformBlockBinding(shaderId, index, newBindingPoint);

    assert(glGetError() == GL_NO_ERROR);
}

auto UniformBlock::fetchIndexInProgram(std::string_view const name) const
    -> int
{
    auto const shaderId = shader->getId();

    return static_cast<int>(glGetUniformBlockIndex(shaderId, name.data()));
}

auto UniformBlock::fetchSize() const
    -> int
{
    auto const shaderId = shader->getId();

    auto blockSize = 0;

    glGetActiveUniformBlockiv(shaderId, index, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

    assert(glGetError() == GL_NO_ERROR);

    return blockSize;
}

} // namespace glow
