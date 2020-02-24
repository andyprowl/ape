#include <Ape/Rendering/Rendering/BodyBoundsShaderProgram.hpp>

#include <Glow/Shader/ShaderBuilder.hpp>
#include <Glow/Shader/ShaderBuilderStreamLogger.hpp>

#include <iostream>

namespace ape
{

namespace
{

auto buildBodyBoundsShader()
    -> glow::ShaderProgram
{
    auto logger = glow::logging::ShaderBuilderStreamLogger{std::cout};

    auto const builder = glow::ShaderBuilder{{resourceFolder "/shaders"}, logger};

    return builder.buildProgram("Bounds/Bounds.Vertex.glsl", "Bounds/Bounds.Fragment.glsl");
}

} // unnamed namespace


BodyBoundsShaderProgram::BodyBoundsShaderProgram()
    : ShaderProgram{buildBodyBoundsShader()}
    , transformation{*this, "transformation"}
{
}

} // namespace ape
