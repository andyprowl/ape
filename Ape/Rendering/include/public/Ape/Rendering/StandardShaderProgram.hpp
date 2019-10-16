#pragma once

#include <Ape/Rendering/CameraUniform.hpp>
#include <Ape/Rendering/DepthMappingUniform.hpp>
#include <Ape/Rendering/LightingUniform.hpp>
#include <Ape/Rendering/LightingViewUniform.hpp>
#include <Ape/Rendering/MaterialUniform.hpp>

#include <Ape/GpuResource/ShaderProgram.hpp>

namespace ape
{

class StandardShaderProgram : public ShaderProgram
{

public:

    StandardShaderProgram();

public:

    Uniform<glm::mat4> modelTransformation;

    Uniform<glm::mat4> cameraTransformation;

    Uniform<glm::mat3> normalTransformation;

    Uniform<Camera> camera;

    Uniform<Lighting> lighting;

    Uniform<LightingView> lightingView;

    Uniform<Material> material;

    Uniform<DepthMapping> depthMapping;

    Uniform<bool> useBlinnPhongModel;

    Uniform<bool> usePercentageCloserFiltering;

};

} // namespace ape
