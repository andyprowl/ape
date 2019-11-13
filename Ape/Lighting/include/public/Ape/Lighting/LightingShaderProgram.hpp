#pragma once

#include <Ape/Lighting/CameraUniform.hpp>
#include <Ape/Lighting/DepthMappingUniform.hpp>
#include <Ape/Lighting/LightSystemUniform.hpp>
#include <Ape/Lighting/LightSystemViewUniform.hpp>
#include <Ape/Lighting/MaterialUniform.hpp>

#include <Ape/Shader/ShaderProgram.hpp>

namespace ape
{

class LightingShaderProgram : public ShaderProgram
{

public:

    LightingShaderProgram();

public:

    Uniform<glm::mat4> modelTransformation;

    Uniform<glm::mat4> cameraTransformation;

    Uniform<glm::mat3> normalTransformation;

    Uniform<Camera> camera;

    Uniform<LightSystem> lightSystem;

    Uniform<LightSystemView> lightSystemView;

    Uniform<Material> material;

    Uniform<DepthMapping> depthMapping;

    Uniform<bool> useBlinnPhongModel;

    Uniform<bool> usePercentageCloserFiltering;

};

} // namespace ape
