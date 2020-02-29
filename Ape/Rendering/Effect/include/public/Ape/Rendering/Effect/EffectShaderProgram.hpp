#pragma once

#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/Texture2dUniform.hpp>

#include <filesystem>

namespace ape
{

class EffectShaderProgram : public glow::ShaderProgram
{

public:

    explicit EffectShaderProgram(std::filesystem::path fragmentShaderPath);

public:

    glow::Uniform<glow::Texture2d> source;

};

} // namespace ape
