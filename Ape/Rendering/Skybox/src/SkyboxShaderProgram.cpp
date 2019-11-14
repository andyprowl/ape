#include <Ape/Rendering/Skybox/SkyboxShaderProgram.hpp>

#include <Glow/Shader/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto buildStandardShader()
    -> glow::ShaderProgram
{
    auto const builder = glow::ShaderBuilder{{resourceFolder "/shaders"}};

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
