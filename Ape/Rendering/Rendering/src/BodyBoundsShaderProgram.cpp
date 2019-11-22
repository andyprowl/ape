#include <Ape/Rendering/Rendering/BodyBoundsShaderProgram.hpp>

#include <Glow/Shader/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto buildBodyBoundsShader()
    -> glow::ShaderProgram
{
    auto const builder = glow::ShaderBuilder{{resourceFolder "/shaders"}};

    return builder.buildProgram("Bounds/Bounds.Vertex.glsl", "Bounds/Bounds.Fragment.glsl");
}

} // unnamed namespace


BodyBoundsShaderProgram::BodyBoundsShaderProgram()
    : ShaderProgram{buildBodyBoundsShader()}
    , transformation{*this, "transformation"}
{
}

} // namespace ape
