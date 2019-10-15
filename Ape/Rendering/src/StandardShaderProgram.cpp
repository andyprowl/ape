#include <Rendering/StandardShaderProgram.hpp>

#include <GpuResource/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto buildStandardShader()
    -> ShaderProgram
{
    auto const builder = ShaderBuilder{{resourceFolder "/shaders"}};

    return builder.buildProgram(
        "Standard/Standard.Vertex.glsl",
        "Standard/Standard.Fragment.glsl");
}

} // unnamed namespace

StandardShaderProgram::StandardShaderProgram()
    : ShaderProgram{buildStandardShader()}
    , modelTransformation{*this, "transform.model"}
    , cameraTransformation{*this, "transform.camera"}
    , normalTransformation{*this, "transform.normal"}
    , camera{*this, "camera"}
    , lighting{*this, "lighting"}
    , lightingView{*this, "lightingView"}
    , material{*this, "material", 0, 1}
    , depthMapping{*this, "depthMapping", 2}
    , useBlinnPhongModel{*this, "useBlinnPhongModel", true}
{
}

} // namespace ape
