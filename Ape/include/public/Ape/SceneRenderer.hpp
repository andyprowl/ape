#pragma once

#include <Ape/CameraUniform.hpp>
#include <Ape/LightingUniform.hpp>

namespace ape
{

class Body;
class BodyPart;
class Scene;
class StandardShaderProgram;

class SceneRenderer
{

public:

    SceneRenderer(
        Scene const & scene,
        StandardShaderProgram & shader,
        glm::vec3 const & backgroundColor);

    auto render() const
        -> void;

    auto getScene() const
        -> Scene const &;

    auto setScene(Scene const & newScene)
        -> void;

private:

    auto clear() const
        -> void;

    auto drawScene() const
        -> void;

    auto drawBody(Body const & body, glm::mat4 const & cameraTransformation) const
        -> void;

    auto drawBodyPart(BodyPart const & part, glm::mat4 const & cameraTransformation) const
        -> void;

    auto drawMesh(Mesh const & mesh) const
        -> void;

private:

    Scene const * scene;

    StandardShaderProgram * shader;

    glm::vec3 backgroundColor;

};

} // namespace ape
