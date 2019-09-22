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

    SceneRenderer(StandardShaderProgram & shader, glm::vec3 const & backgroundColor);

    auto render(Scene const & s) const
        -> void;

private:

    auto clear() const
        -> void;

    auto drawScene(Scene const & scene) const
        -> void;

    auto drawBody(Body const & body) const
        -> void;

    auto drawBodyPart(BodyPart const & part) const
        -> void;

    auto drawMesh(Mesh const & mesh) const
        -> void;

private:

    StandardShaderProgram * shader;

    glm::vec3 backgroundColor;

};

} // namespace ape
