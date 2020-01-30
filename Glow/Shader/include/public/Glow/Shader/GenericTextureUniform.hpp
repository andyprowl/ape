#pragma once

#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/Uniform.hpp>

#include <string_view>

namespace glow::detail
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

        sampler.set(newTextureUnit);
    }

    auto set(ValueType const & texture)
        -> void
    {
        texture.activate(textureUnit);
    }

    auto getUnit() const
        -> int
    {
        return textureUnit;
    }

public:

    int textureUnit;

    Uniform<int> sampler;

};

} // namespace glow::detail
