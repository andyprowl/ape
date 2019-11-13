#pragma once

#include <Ape/Shader/ShaderProgram.hpp>
#include <Ape/Shader/Uniform.hpp>

#include <string_view>

namespace ape::detail
{

template<typename TextureType>
class GenericTextureUniform
{

public:

    using ValueType = TextureType;

public:
 
    GenericTextureUniform(ShaderProgram & program, std::string_view name)
        : textureUnit{-1}
        , sampler{program, name}
    {
    }

    GenericTextureUniform(ShaderProgram & program, std::string_view name, int const textureUnit)
        : textureUnit{textureUnit}
        , sampler{program, name, textureUnit}
    {
    }

    auto bind(int const newTextureUnit)
        -> void
    {
        textureUnit = newTextureUnit;

        sampler = newTextureUnit;
    }

    auto set(ValueType const & texture)
        -> void
    {
        texture.activate(textureUnit);
    }

    auto operator = (ValueType const & texture)
        -> GenericTextureUniform &
    {
        set(texture);

        return *this;
    }

public:

    int textureUnit;

    Uniform<int> sampler;

};

} // namespace ape::detail
