#pragma once

#include <Ape/GpuResource/ShaderProgram.hpp>
#include <Ape/GpuResource/TextureUniform.hpp>

#include <filesystem>

namespace ape
{

class EffectShaderProgram : public ShaderProgram
{

public:

    explicit EffectShaderProgram(std::filesystem::path fragmentShaderPath);

public:

    Uniform<Texture> source;

};

} // namespace ape
