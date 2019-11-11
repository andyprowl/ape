#include <Ape/Rendering/StandardShaderProgram.hpp>

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
    , lightSystem{*this, "lightSystem"}
    , lightSystemView{*this, "lightSystemView"}
    , material{*this, "material", 0, 1}
    , depthMapping{*this, "depthMapping", 2}
    , useBlinnPhongModel{*this, "useBlinnPhongModel", true}
    , usePercentageCloserFiltering{*this, "usePercentageCloserFiltering", false}
{
}

} // namespace ape
