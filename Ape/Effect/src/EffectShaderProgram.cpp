#include <Ape/Effect/EffectShaderProgram.hpp>

#include <Glow/Shader/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto const screenTextureUnit = 0;

auto buildVertexShader()
    -> glow::VertexShader
{
    auto const builder = glow::ShaderBuilder{{resourceFolder "/shaders/Effects"}};

    return builder.buildVertexShader("Effect.Vertex.glsl");
}

auto buildFragmentShader(std::filesystem::path fragmentShaderPath)
    -> glow::FragmentShader
{
    auto const builder = glow::ShaderBuilder{{fragmentShaderPath.parent_path()}};

    return builder.buildFragmentShader(fragmentShaderPath.filename());
}

} // unnamed namespace

EffectShaderProgram::EffectShaderProgram(std::filesystem::path fragmentShaderPath)
    : ShaderProgram{buildVertexShader(), buildFragmentShader(std::move(fragmentShaderPath))}
    , source{*this, "screenTexture", screenTextureUnit}
{
}

} // namespace ape
