#pragma once

#include <Glow/Shader/ShaderProgram.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class BodyBoundsShaderProgram : public glow::ShaderProgram
{

public:

    BodyBoundsShaderProgram();

public:

    glow::Uniform<glm::mat4> transformation;

};

} // namespace ape
