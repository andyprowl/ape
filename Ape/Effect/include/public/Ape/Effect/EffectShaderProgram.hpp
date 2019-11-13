#pragma once

#include <Ape/Shader/ShaderProgram.hpp>
#include <Ape/Shader/TextureUniform.hpp>

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
