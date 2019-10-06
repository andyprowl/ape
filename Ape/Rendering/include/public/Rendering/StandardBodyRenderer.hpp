#pragma once

#include <Scene/BodyRange.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class Body;
class BodyPart;
class Camera;
class Lighting;
class Mesh;
class ShapeRenderer;
class Rendering;
class StandardShaderProgram;

class StandardBodyRenderer
{

public:

    StandardBodyRenderer(StandardShaderProgram & shader, ShapeRenderer const & shapeRenderer);

    // This function gets invoked once before the frame rendering starts.
    auto prepare(Camera const & camera, Lighting const & lighting) const
        -> void;

    // This function may be invoked more than once during the same frame rendering.
    auto render(BodyRange const & bodies, Camera const & camera) const
        -> void;

private:

    auto renderBody(Body const & body, glm::mat4 const & transformation) const
        -> void;

    auto renderBodyPart(BodyPart const & part, glm::mat4 const & cameraTransformation) const
        -> void;

    auto renderMesh(Mesh const & mesh) const
        -> void;

private:

    StandardShaderProgram * shader;

    ShapeRenderer const * shapeRenderer;

};

} // namespace ape
