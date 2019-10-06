#pragma once

#include <Rendering/LineStyle.hpp>

#include <Scene/BodyRange.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class Body;
class BodyPart;
class Camera;
class Mesh;
class ShapeRenderer;
class WireframeShaderProgram;

class WireframeBodyRenderer
{

public:

    WireframeBodyRenderer(WireframeShaderProgram & shader, ShapeRenderer const & shapeRenderer);

    auto render(
        BodyRange const & bodies,
        Camera const & camera,
        LineStyle const & lineStyle) const
        -> void;

private:

    auto renderBody(Body const & body, glm::mat4 const & transformation) const
        -> void;

    auto renderBodyPart(BodyPart const & part, glm::mat4 const & cameraTransformation) const
        -> void;

    auto renderMesh(Mesh const & mesh) const
        -> void;

private:

    WireframeShaderProgram * shader;

    ShapeRenderer const * shapeRenderer;

};

} // namespace ape
