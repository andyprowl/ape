#pragma once

#include <Ape/Lighting/PointLightViewUniform.hpp>

#include <Glow/Shader/ShaderProgram.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class OmniDepthShaderProgram : public glow::ShaderProgram
{

public:

    OmniDepthShaderProgram();

public:

    glow::Uniform<glm::mat4> modelTransformation;

    // TODO: Is it OK for this to be named PointLightView? Shouldn't it be something generic, like
    // "OmnidirectionalView"? After all, depth mapping does not necessarily have to be done for a
    // light. The same then applies to the MonoDepthShaderProgram (which should be renamed to
    // MonoDepthShaderProgram): instead of lightTransformation, we should use viewTransformation.
    // The problem with the word "view", though, is that it does not encompass "projection",
    // whereas the transformations we are passing here do.
    // But in this case we also a problem with the name "LightSystemView", because the
    // transformations maintained there also encompass projection.
    glow::Uniform<PointLightView> lightTransformation;

    // TODO: Same as above: is it OK to mention "light" here?
    glow::Uniform<glm::vec3> lightPosition;

};

} // namespace ape
