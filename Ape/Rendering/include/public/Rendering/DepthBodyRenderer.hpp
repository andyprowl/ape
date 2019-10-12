#pragma once

#include <Rendering/DepthMap.hpp>

#include <Scene/BodyRange.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class Body;
class BodyPart;
class Camera;
class DepthMapping;
class DepthShaderProgram;
class LightingView;
class Mesh;
class ShapeRenderer;

class DepthBodyRenderer
{

public:

    using BodySetView = ContainerView<std::vector<Body>>;

public:

    DepthBodyRenderer(DepthShaderProgram & shader, ShapeRenderer const & shapeRenderer);

    auto render(
        BodySetView const & bodies,
        LightingView const & lightingView,
        DepthMapping & target) const
        -> void;

private:

    auto render(BodySetView const & bodies, Camera const & lightView, DepthMap & target) const
        -> void;

    auto renderBody(Body const & body, glm::mat4 const & lightTransformation) const
        -> void;

    auto renderBodyPart(BodyPart const & part, glm::mat4 const & lightTransformation) const
        -> void;

    auto renderMesh(Mesh const & mesh) const
        -> void;

private:

    DepthShaderProgram * shader;

    ShapeRenderer const * shapeRenderer;

};

} // namespace ape
