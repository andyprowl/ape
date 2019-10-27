#include <Ape/Rendering/EffectShaderProgram.hpp>

#include <Ape/GpuResource/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto const screenTextureUnit = 0;

auto buildFlatQuadShader(std::string fragmentShaderFileName)
    -> ShaderProgram
{
    auto const builder = ShaderBuilder{{resourceFolder "/shaders/Effects"}};

    return builder.buildProgram("Quad.Vertex.glsl", std::move(fragmentShaderFileName));
}

} // unnamed namespace

EffectShaderProgram::EffectShaderProgram(std::string fragmentShaderFileName)
    : ShaderProgram{buildFlatQuadShader(std::move(fragmentShaderFileName))}
    , source{*this, "screenTexture", screenTextureUnit}
{
}

} // namespace ape
