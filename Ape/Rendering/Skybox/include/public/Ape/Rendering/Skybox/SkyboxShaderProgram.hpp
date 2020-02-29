#pragma once

#include <Ape/Rendering/Lighting/FogUniform.hpp>

#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/TextureCubeUniform.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class SkyboxShaderProgram : public glow::ShaderProgram
{

public:

    SkyboxShaderProgram();

    glow::Uniform<glm::mat4> transform;

    glow::Uniform<glow::TextureCube> skybox;

    glow::Uniform<glm::vec3> cameraPosition;

    glow::Uniform<Fog> fog;

};

} // namespace ape
