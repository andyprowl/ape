#pragma once

#include <Rendering/DepthBodyRenderer.hpp>
#include <Rendering/Viewport.hpp>

#include <Scene/BodyRange.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class Body;
class BodyPart;
class Camera;
class DepthShaderProgram;
class Lighting;
class Mesh;
class ShapeRenderer;
class Rendering;
class StandardShaderProgram;

class StandardBodyRenderer
{

public:

    StandardBodyRenderer(
        StandardShaderProgram & shader,
        DepthShaderProgram & depthShader,
        ShapeRenderer const & shapeRenderer);

    auto render(
        BodyRange const & bodies,
        Camera const & camera,
        Lighting const & lighting) const
        -> void;

    auto setViewport(Viewport const & newViewport)
        -> void;

private:

    auto renderDepthMap(BodyRange const & bodies, Camera const & lightView) const
        -> void;

    auto renderRegularScene(
        BodyRange const & bodies,
        Camera const & camera,
        Lighting const & lighting,
        Camera const & lightView) const
        -> void;

    auto renderBody(
        Body const & body,
        glm::mat4 const & cameraTransformation,
        glm::mat4 const & lightTransformation) const
        -> void;

    auto renderBodyPart(
        BodyPart const & part,
        glm::mat4 const & cameraTransformation,
        glm::mat4 const & lightTransformation) const
        -> void;

    auto renderMesh(Mesh const & mesh) const
        -> void;

private:

    StandardShaderProgram * shader;

    DepthBodyRenderer depthMapRenderer;

    ShapeRenderer const * shapeRenderer;

    Viewport viewport;

};

} // namespace ape
