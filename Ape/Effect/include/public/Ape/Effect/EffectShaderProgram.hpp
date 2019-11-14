#pragma once

#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/TextureUniform.hpp>

#include <filesystem>

namespace ape
{

class EffectShaderProgram : public glow::ShaderProgram
{

public:

    explicit EffectShaderProgram(std::filesystem::path fragmentShaderPath);

public:

    glow::Uniform<glow::Texture> source;

};

} // namespace ape
