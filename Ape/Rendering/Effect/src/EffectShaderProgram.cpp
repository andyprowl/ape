#include <Ape/Rendering/Effect/EffectShaderProgram.hpp>

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

    return builder.buildVertexShader("Effect.Vertex.glsl", "Effect.Vertex");
}

auto buildFragmentShader(std::filesystem::path fragmentShaderPath)
    -> glow::FragmentShader
{
    auto const builder = glow::ShaderBuilder{{fragmentShaderPath.parent_path()}};

    return builder.buildFragmentShader(fragmentShaderPath.filename(), "Effect.Fragment");
}

} // unnamed namespace

EffectShaderProgram::EffectShaderProgram(std::filesystem::path fragmentShaderPath)
    : ShaderProgram{
        buildVertexShader(),
        buildFragmentShader(std::move(fragmentShaderPath)),
        "Effect"}
    , source{*this, "screenTexture", screenTextureUnit}
{
}

} // namespace ape
