#pragma once

#include <Glow/Shader/ShaderProgram.hpp>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace ape
{

class WireframeShaderProgram : public glow::ShaderProgram
{

public:

    WireframeShaderProgram();

public:

    glow::Uniform<glm::mat4> transformation;

    glow::Uniform<glm::vec3> lineColor;

    glow::Uniform<float> lineWidth;

};

} // namespace ape
