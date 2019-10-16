#pragma once

#include <Rendering/DepthMap.hpp>

#include <Scene/BodyRange.hpp>

#include <glm/mat4x4.hpp>

#include <vector>

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

    template<typename LightType>
    auto render(
        BodySetView const & bodies,
        std::vector<LightType> const & lights,
        std::vector<glm::mat4> const & lightViews,
        std::vector<DepthMap> & depthMaps) const
        -> void;

    auto render(
        BodySetView const & bodies,
        glm::mat4 const & lightTransformation,
        DepthMap & target) const
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
