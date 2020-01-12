#include <Ape/Rendering/Lighting/BlinnPhongShaderProgram.hpp>

#include <Glow/Shader/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto buildBlinnPhongShader()
    -> glow::ShaderProgram
{
    auto const builder = glow::ShaderBuilder{{resourceFolder "/shaders"}};

    return builder.buildProgram(
        std::vector<glow::VertexShaderPath>{"BlinnPhong/BlinnPhong.Vertex.glsl"},
        std::vector<glow::GeometryShaderPath>{},
        std::vector<glow::FragmentShaderPath>{
            "BlinnPhong/BlinnPhong.Fragment.glsl",
            "Fog/Fog.Fragment.glsl"},
        std::string{"BlinnPhong"});
}

} // unnamed namespace

BlinnPhongShaderProgram::BlinnPhongShaderProgram()
    : ShaderProgram{buildBlinnPhongShader()}
    , worldTransformation{*this, "transform.model"}
    , cameraTransformation{*this, "transform.camera"}
    , normalTransformation{*this, "transform.normal"}
    , cameraPosition{*this, "camera.position"}
    , lightSystem{*this, "lightSystem"}
    , lightSystemView{*this, "lightSystemView"}
    , material{*this, "material", 0, 1, 2}
    , depthMapping{*this, "depthMapping", 3}
    , usePhongModel{*this, "usePhongModel", false}
    , usePercentageCloserFiltering{*this, "usePercentageCloserFiltering", false}
    , useNormalMapping{*this, "useNormalMapping", true}
    , fogDensity{*this, "fogDensity", 0.0f}
    , fogColor{*this, "fogColor", glm::vec3{0.0f, 0.0f, 0.0f}}
{
}

} // namespace ape
