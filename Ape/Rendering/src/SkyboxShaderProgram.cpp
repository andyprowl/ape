#include <Ape/Rendering/SkyboxShaderProgram.hpp>

#include <Ape/GpuResource/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto buildStandardShader()
    -> ShaderProgram
{
    auto const builder = ShaderBuilder{{resourceFolder "/shaders"}};

    return builder.buildProgram(
        "Skybox/Skybox.Vertex.glsl",
        "Skybox/Skybox.Fragment.glsl");
}

} // unnamed namespace

SkyboxShaderProgram::SkyboxShaderProgram()
    : ShaderProgram{buildStandardShader()}
    , transform{*this, "transform"}
    , skybox{*this, "skybox", 0}
{
}

} // namespace ape
