#pragma once

#include <Ape/CameraUniform.hpp>
#include <Ape/LightingUniform.hpp>

#include <stdexcept>

namespace ape
{

class Body;
class BodyPart;
class Camera;
class CameraSelector;
class Scene;
class StandardShaderProgram;

class SceneRenderer
{

public:

    SceneRenderer(
        CameraSelector const & cameraSelector,
        StandardShaderProgram & shader,
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

    auto drawScene(Scene const & scene, Camera const & camera) const
        -> void;

    auto drawBody(Body const & body, glm::mat4 const & cameraTransformation) const
        -> void;

    auto drawBodyPart(BodyPart const & part, glm::mat4 const & cameraTransformation) const
        -> void;

    auto drawMesh(Mesh const & mesh) const
        -> void;

private:

    CameraSelector const * cameraSelector;

    StandardShaderProgram * shader;

    glm::vec3 backgroundColor;

};

auto getCamera(SceneRenderer const & renderer)
    -> Camera *;

auto getScene(SceneRenderer const & renderer)
    -> Scene &;

} // namespace ape
