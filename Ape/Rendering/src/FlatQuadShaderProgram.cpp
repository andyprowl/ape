#include <Ape/Rendering/FlatQuadShaderProgram.hpp>

#include <Ape/GpuResource/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto const screenTextureUnit = 0;

auto buildFlatQuadShader()
    -> ShaderProgram
{
    auto const builder = ShaderBuilder{{resourceFolder "/shaders"}};

    return builder.buildProgram("FlatQuad/Quad.Vertex.glsl", "FlatQuad/Quad.Fragment.glsl");
}

} // unnamed namespace

FlatQuadShaderProgram::FlatQuadShaderProgram()
    : ShaderProgram{buildFlatQuadShader()}
    , source{*this, "screenTexture", screenTextureUnit}
{
}

} // namespace ape
