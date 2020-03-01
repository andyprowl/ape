#include <Ape/Rendering/Lighting/BlinnPhongShaderProgram.hpp>

#include <Glow/Shader/ShaderBuilder.hpp>
#include <Glow/Shader/ShaderBuilderStreamLogger.hpp>

#include <iostream>

namespace ape
{

namespace
{

auto const lightSystemBlockBindingPoint = 0;

auto const lightSystemViewBlockBindingPoint = 1;

auto const materialSetBindingPoint = 2;

auto buildBlinnPhongShader()
    -> glow::ShaderProgram
{
    auto logger = glow::logging::ShaderBuilderStreamLogger{std::cout};

    auto const builder = glow::ShaderBuilder{{resourceFolder "/shaders"}, logger};

    return builder.buildProgram(
        std::vector<glow::VertexShaderPath>{"BlinnPhong/BlinnPhong.Vertex.glsl"},
        std::vector<glow::GeometryShaderPath>{},
        std::vector<glow::FragmentShaderPath>{
            "BlinnPhong/BlinnPhong.Fragment.glsl",
            "Shadow/Shadow.Fragment.glsl",
            "Fog/Fog.Fragment.glsl"},
        std::string{"BlinnPhong"});
}

} // unnamed namespace

BlinnPhongShaderProgram::BlinnPhongShaderProgram()
    : ShaderProgram{buildBlinnPhongShader()}
    , objectToWorldTransformation{*this, "transform.objectToWorld"}
    , worldToClipTransformation{*this, "transform.worldToClip"}
    , normalTransformation{*this, "transform.normal"}
    , cameraPosition{*this, "camera.position"}
    , lightSystem{*this, "LightSystemUniformBlock"}
    , lightSystemView{*this, "LightSystemViewUniformBlock"}
    , materialSet{*this, "MaterialSetBlock"}
    , materialMaps{*this, "materialMaps", 0, 1, 2}
    , activeMaterialIndex{*this, "activeMaterialIndex"}
    , fog{*this, "fog"}
    , depthMapping{*this, "depthMapping", 3}
    , usePhongModel{*this, "usePhongModel", false}
    , usePercentageCloserFiltering{*this, "usePercentageCloserFiltering", false}
    , useNormalMapping{*this, "useNormalMapping", true}
{
    lightSystem.setBindingPoint(lightSystemBlockBindingPoint);

    lightSystemView.setBindingPoint(lightSystemViewBlockBindingPoint);

    materialSet.setBindingPoint(materialSetBindingPoint);
}

} // namespace ape
