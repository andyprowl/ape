#pragma once

#include <Core/CameraUniform.hpp>
#include <Core/LightingUniform.hpp>

#include <Asset/RenderingContext.hpp>

#include <Glow/VertexArrayObject.hpp>

#include <stdexcept>

namespace ape
{

class Body;
class BodyPart;
class BodySelector;
class Camera;
class CameraSelector;
class WireframeShaderProgram;
class Scene;
class StandardShaderProgram;

class SceneRenderer
{

public:

    SceneRenderer(
        RenderingContext const & renderingContext,
        CameraSelector const & cameraSelector,
        BodySelector const & pickedBodySelector,
        StandardShaderProgram & standardShader,
        WireframeShaderProgram & pickingShader,
        glm::vec3 const & backgroundColor);

    auto render() const
        -> void;

    auto getCameraSelector() const
        -> CameraSelector const &;

    auto setCameraSelector(CameraSelector const & newSelector)
        -> void;

private:

    auto clear() const
        -> void;

    auto setupLighting() const
        -> void;

    auto setupCamera(Camera const & camera) const
        -> void;

    auto drawBodies(Camera const & camera) const
        -> void;

    auto drawBodiesWithStandardShader(
        Camera const & camera,
        std::vector<Body *> const & bodies) const
        -> void;

    auto drawBodiesWithPickingShader(
        Camera const & camera,
        std::vector<Body *> const & bodies) const
        -> void;

    auto drawBodyWithStandardShader(Body const & body, glm::mat4 const & transformation) const
        -> void;

    auto drawBodyPartWithStandardShader(
        BodyPart const & part,
        glm::mat4 const & transformation) const
        -> void;

    auto drawMeshWithStandardShader(Mesh const & mesh) const
        -> void;

private:

    RenderingContext renderingContext;

    CameraSelector const * cameraSelector;

    BodySelector const * pickedBodySelector;

    StandardShaderProgram * standardShader;

    WireframeShaderProgram * pickingShader;

    glm::vec3 backgroundColor;

    VertexArrayObject arrayObject;

};

auto getCamera(SceneRenderer const & renderer)
    -> Camera *;

auto getScene(SceneRenderer const & renderer)
    -> Scene &;

} // namespace ape
