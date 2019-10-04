#pragma once

#include <Engine/CameraUniform.hpp>
#include <Engine/LightingUniform.hpp>

#include <GpuResource/ShaderProgram.hpp>

namespace ape
{

class StandardShaderProgram : public ShaderProgram
{

public:

    StandardShaderProgram();

public:

    Uniform<Camera> camera;

    Uniform<Lighting> lighting;

    Uniform<glm::mat4> modelTransformation;

    Uniform<glm::mat4> cameraTransformation;

    Uniform<glm::mat3> normalTransformation;

    Uniform<glm::vec3> materialAmbient;

    Uniform<float> materialShininess;

    Uniform<bool> useBlinnPhongModel;

};

} // namespace ape
