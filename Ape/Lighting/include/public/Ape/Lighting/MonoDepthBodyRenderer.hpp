#pragma once

#include <Ape/Lighting/MonoDepthMap.hpp>

#include <Ape/Scene/BodyRange.hpp>

#include <glm/mat4x4.hpp>

#include <vector>

namespace ape
{

class Body;
class BodyPart;
class Camera;
class DepthMapping;
class MonoDepthShaderProgram;
class LightSystemView;
class Mesh;
class ShapeDrawer;

class MonoDepthBodyRenderer
{

public:

    using BodySetView = ContainerView<std::vector<Body>>;

public:

    MonoDepthBodyRenderer(MonoDepthShaderProgram & shader, ShapeDrawer const & shapeRenderer);

    auto render(
        BodySetView const & bodies,
        LightSystemView const & lightSystemView,
        DepthMapping & target) const
        -> void;

private:

    auto renderSpotLightSetDepth(
        BodySetView const & bodies,
        LightSystemView const & lightSystemView,
        DepthMapping & target) const
        -> void;

    auto renderDirectionalLightSetDepth(
        BodySetView const & bodies,
        LightSystemView const & lightSystemView,
        DepthMapping & target) const
        -> void;

    template<typename LightType>
    auto renderLightSetDepth(
        BodySetView const & bodies,
        std::vector<LightType> const & lights,
        std::vector<glm::mat4> const & lightViews,
        std::vector<MonoDepthMap> & depthMaps) const
        -> void;

    auto renderLightDepth(
        BodySetView const & bodies,
        glm::mat4 const & lightTransformation,
        MonoDepthMap & target) const
        -> void;

    auto renderBody(Body const & body, glm::mat4 const & lightTransformation) const
        -> void;

    auto renderBodyPart(BodyPart const & part, glm::mat4 const & lightTransformation) const
        -> void;

    auto renderMesh(Mesh const & mesh) const
        -> void;

private:

    MonoDepthShaderProgram * shader;

    ShapeDrawer const * shapeRenderer;

};

} // namespace ape
