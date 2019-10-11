#pragma once

#include <Rendering/DepthMap.hpp>

#include <Scene/BodyRange.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class Body;
class BodyPart;
class Camera;
class Mesh;
class ShapeRenderer;
class DepthShaderProgram;

class DepthBodyRenderer
{

public:

    using BodyContainerView = ContainerView<std::vector<Body>>;

public:

    DepthBodyRenderer(DepthShaderProgram & shader, ShapeRenderer const & shapeRenderer);

    auto render(BodyRange const & bodies, Camera const & lightView) const
        -> void;

    auto render(BodyContainerView const & bodies, Camera const & lightView) const
        -> void;

    auto getDepthMap() const
        -> DepthMap const &;

    auto getDepthMapSize() const
        -> Size<int>;

private:

    template<typename Range>
    auto renderBodies(Range const & bodies, Camera const & lightView) const
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

    DepthMap depthMap;

};

} // namespace ape
