#pragma once

#include <Glow/Shader/ShaderProgram.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class MonoDepthShaderProgram : public glow::ShaderProgram
{

public:

    MonoDepthShaderProgram();

public:

    glow::Uniform<glm::mat4> lightTransformation;

};

} // namespace ape
