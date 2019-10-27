#pragma once

#include <Ape/GpuResource/ShaderProgram.hpp>
#include <Ape/GpuResource/TextureUniform.hpp>

#include <string>

namespace ape
{

class EffectShaderProgram : public ShaderProgram
{

public:

    explicit EffectShaderProgram(std::string fragmentShaderFileName);

public:

    Uniform<Texture> source;

};

} // namespace ape
