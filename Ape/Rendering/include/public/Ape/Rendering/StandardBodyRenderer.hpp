#pragma once

#include <Ape/Rendering/DepthBodyRenderer.hpp>
#include <Ape/Rendering/Viewport.hpp>

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
class ShapeRenderer;
class StandardShaderProgram;

class StandardBodyRenderer
{

public:

    StandardBodyRenderer(StandardShaderProgram & shader, ShapeRenderer const & shapeRenderer);
 
    auto setViewport(Viewport const & newViewport)
        -> void;

    auto render(
        BodyRange const & bodies,
        Camera const & camera,
        LightSystem const & lightSystem,
        ShadowMapping const & shadowMapping) const
        -> void;

private:

    auto setupViewport() const
        -> void;

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

    StandardShaderProgram * shader;

    ShapeRenderer const * shapeRenderer;

    Viewport viewport;

};

} // namespace ape
