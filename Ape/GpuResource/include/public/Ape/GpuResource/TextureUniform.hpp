#pragma once

#include <GpuResource/ShaderProgram.hpp>
#include <GpuResource/Texture.hpp>

namespace ape
{

template<>
class Uniform<Texture>
{

public:

    using ValueType = Texture;

public:
 
    Uniform(ShaderProgram & program, std::string const & name)
        : textureUnit{-1}
        , sampler{program, name}
    {
    }

    Uniform(ShaderProgram & program, std::string const & name, int const textureUnit)
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
        texture.bind(textureUnit);
    }

    auto operator = (ValueType const & texture)
        -> Uniform &
    {
        set(texture);

        return *this;
    }

public:

    int textureUnit;

    Uniform<int> sampler;

};

} // namespace ape
