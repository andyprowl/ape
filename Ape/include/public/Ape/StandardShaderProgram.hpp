#pragma once

#include <Ape/CameraUniform.hpp>
#include <Ape/LightingUniform.hpp>
#include <Ape/ShaderProgram.hpp>

class StandardShaderProgram : public ShaderProgram
{

public:

    StandardShaderProgram();

public:

    Uniform<Camera> camera;

    Uniform<Lighting> lighting;

    Uniform<glm::mat4> modelTransformation;

    Uniform<glm::mat3> normalMatrix;

    Uniform<glm::vec3> materialAmbient;

    Uniform<float> materialShininess;

    Uniform<bool> useBlinnPhongModel;

};
