#include <Ape/Rendering/EffectShaderProgram.hpp>

#include <Ape/GpuResource/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto const screenTextureUnit = 0;

auto buildVertexShader()
    -> VertexShader
{
    auto const builder = ShaderBuilder{{resourceFolder "/shaders/Effects"}};

    return builder.buildVertexShader("Effect.Vertex.glsl");
}

auto buildFragmentShader(std::filesystem::path fragmentShaderPath)
    -> FragmentShader
{
    auto const builder = ShaderBuilder{{fragmentShaderPath.parent_path()}};

    return builder.buildFragmentShader(fragmentShaderPath.filename());
}

} // unnamed namespace

EffectShaderProgram::EffectShaderProgram(std::filesystem::path fragmentShaderPath)
    : ShaderProgram{buildVertexShader(), buildFragmentShader(std::move(fragmentShaderPath))}
    , source{*this, "screenTexture", screenTextureUnit}
{
}

} // namespace ape
