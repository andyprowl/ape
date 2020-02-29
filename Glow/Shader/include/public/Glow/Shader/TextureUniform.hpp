#pragma once

#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/Uniform.hpp>

#include <Glow/Texture/TextureUnitSet.hpp>

#include <string_view>

namespace glow::detail
{

template<typename TextureType>
class TextureUniform
{

public:

    using ValueType = TextureType;

public:
 
    TextureUniform(ShaderProgram & program, std::string_view name)
        : textureUnit{nullptr}
        , sampler{program, name}
    {
    }

    TextureUniform(ShaderProgram & program, std::string_view name, int const textureUnit)
        : textureUnit{&textureUnits[textureUnit]}
        , sampler{program, name, textureUnit}
    {
    }

    auto setTextureUnit(int const newTextureUnit)
        -> void
    {
        textureUnit = &textureUnits[newTextureUnit];

        sampler.set(newTextureUnit);
    }

    auto set(ValueType const & texture)
        -> void
    {
        textureUnit->setTexture(&texture);
    }

    auto getUnit() const
        -> int
    {
        return textureUnit;
    }

public:

    TextureUnit * textureUnit;

    Uniform<int> sampler;

};

} // namespace glow::detail
