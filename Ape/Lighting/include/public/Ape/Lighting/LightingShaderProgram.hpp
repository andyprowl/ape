#pragma once

#include <Ape/Lighting/CameraUniform.hpp>
#include <Ape/Lighting/DepthMappingUniform.hpp>
#include <Ape/Lighting/LightSystemUniform.hpp>
#include <Ape/Lighting/LightSystemViewUniform.hpp>
#include <Ape/Lighting/MaterialUniform.hpp>

#include <Glow/Shader/ShaderProgram.hpp>

namespace ape
{

class LightingShaderProgram : public glow::ShaderProgram
{

public:

    LightingShaderProgram();

public:

    glow::Uniform<glm::mat4> modelTransformation;

    glow::Uniform<glm::mat4> cameraTransformation;

    glow::Uniform<glm::mat3> normalTransformation;

    glow::Uniform<Camera> camera;

    glow::Uniform<LightSystem> lightSystem;

    glow::Uniform<LightSystemView> lightSystemView;

    glow::Uniform<Material> material;

    glow::Uniform<DepthMapping> depthMapping;

    glow::Uniform<bool> useBlinnPhongModel;

    glow::Uniform<bool> usePercentageCloserFiltering;

};

} // namespace ape
