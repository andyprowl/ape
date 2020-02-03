#include <Glow/Shader/UniformBlock.hpp>

#include <Glow/Shader/ShaderProgram.hpp>

#include <Glow/BufferObject/UniformBufferObject.hpp>

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

    auto const blockIndex = static_cast<int>(glGetUniformBlockIndex(shaderId, name.data()));

    assert(blockIndex >= 0);

    assert(glGetError() == GL_NO_ERROR);
    
    return blockIndex;
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

auto setBlockDataSource(UniformBufferObject & buffer, UniformBlock const & block)
    -> void
{
    auto const bindingPoint = block.getBindingPoint();

    buffer.bind(bindingPoint);
}

} // namespace glow
