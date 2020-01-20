#pragma once

#include <Ape/Rendering/Wireframe/LineStyle.hpp>

#include <Ape/World/Scene/BodyRange.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class Body;
class BodyPart;
class Camera;
class LineStyleProvider;
class Mesh;
class ShapeDrawer;
class WireframeShaderProgram;

class WireframeBodyRenderer
{

public:

    WireframeBodyRenderer(
        WireframeShaderProgram & shader,
        ShapeDrawer & shapeRenderer,
        LineStyleProvider const & styleProvider);

    auto render(BodyRange const & bodies, Camera const & camera) const
        -> void;

private:

    auto setupLineStyleUniforms() const
        -> void;

    auto renderBody(Body const & body, glm::mat4 const & transformation) const
        -> void;

    auto renderBodyPart(BodyPart const & part, glm::mat4 const & cameraTransformation) const
        -> void;

    auto renderMesh(Mesh const & mesh) const
        -> void;

private:

    WireframeShaderProgram * shader;

    ShapeDrawer * shapeRenderer;
    
    LineStyleProvider const * styleProvider;

};

} // namespace ape
