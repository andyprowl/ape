#pragma once

#include <Ape/Lighting/DepthBodyRenderer.hpp>

#include <Ape/Scene/BodyRange.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class Body;
class BodyPart;
class Camera;
class LightSystem;
class Mesh;
class ShadowMapping;
class ShapeDrawer;
class LightingShaderProgram;

class LightingBodyRenderer
{

public:

    LightingBodyRenderer(LightingShaderProgram & shader, ShapeDrawer const & shapeRenderer);

    auto render(
        BodyRange const & bodies,
        Camera const & camera,
        LightSystem const & lightSystem,
        ShadowMapping const & shadowMapping) const
        -> void;

private:

    auto setupInvariantUniforms(
        Camera const & camera,
        LightSystem const & lightSystem,
        ShadowMapping const & shadowMapping) const
        -> void;

    auto renderBody(Body const & body, glm::mat4 const & cameraTransformation) const
        -> void;

    auto renderBodyPart(BodyPart const & part, glm::mat4 const & cameraTransformation) const
        -> void;

    auto setupBodyPartUniforms(
        BodyPart const & part,
        glm::mat4 const & cameraTransformation) const
        -> void;

    auto renderMesh(Mesh const & mesh) const
        -> void;

private:

    LightingShaderProgram * shader;

    ShapeDrawer const * shapeRenderer;

};

} // namespace ape
