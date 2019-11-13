#pragma once

#include <Ape/Shader/CubeTextureUniform.hpp>
#include <Ape/Shader/ShaderProgram.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class SkyboxShaderProgram : public ShaderProgram
{

public:

    SkyboxShaderProgram();

    Uniform<glm::mat4> transform;

    Uniform<CubeTexture> skybox;

};

} // namespace ape
