#pragma once

#include <Ape/Rendering/Lighting/CameraUniform.hpp>
#include <Ape/Rendering/Lighting/DepthMappingUniform.hpp>
#include <Ape/Rendering/Lighting/LightSystemUniform.hpp>
#include <Ape/Rendering/Lighting/LightSystemViewUniform.hpp>
#include <Ape/Rendering/Lighting/MaterialUniform.hpp>

#include <Glow/Shader/ShaderProgram.hpp>

namespace ape
{

class LightingShaderProgram : public glow::ShaderProgram
{

public:

    LightingShaderProgram();

public:

    glow::Uniform<glm::mat4> worldTransformation;

    glow::Uniform<glm::mat4> cameraTransformation;

    glow::Uniform<glm::mat3> normalTransformation;

    glow::Uniform<Camera> camera;

    glow::Uniform<LightSystem> lightSystem;

    glow::Uniform<LightSystemView> lightSystemView;

    glow::Uniform<Material> material;

    glow::Uniform<DepthMapping> depthMapping;

    glow::Uniform<bool> usePhongModel;

    glow::Uniform<bool> usePercentageCloserFiltering;

};

} // namespace ape
