#pragma once

#include <Ape/Rendering/Lighting/MonoDepthMap.hpp>

#include <Ape/World/Scene/BodyRange.hpp>

#include <glm/mat4x4.hpp>

#include <vector>

namespace ape
{

class Body;
class BodyPart;
class BodyPartMesh;
class Camera;
class DepthMapping;
class FrustumCuller;
class MonoDepthShaderProgram;
class LightSystemView;
class Mesh;
class ShapeDrawer;

class MonoDepthBodyRenderer
{

public:

    using BodySetView = basix::ContainerView<std::vector<Body>>;

public:

    MonoDepthBodyRenderer(MonoDepthShaderProgram & shader, ShapeDrawer const & shapeRenderer);

    auto render(
        BodySetView const & bodies,
        LightSystemView const & lightSystemView,
        DepthMapping & target) const
        -> void;

    auto isFrustumCullingEnabled() const
        -> bool;

    auto enableFrustumCulling(bool enable)
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

    template<typename LightType, typename LightViewType>
    auto renderLightSetDepth(
        BodySetView const & bodies,
        std::vector<LightType> const & lights,
        std::vector<LightViewType> const & lightViews,
        std::vector<MonoDepthMap> & depthMaps) const
        -> void;

    template<typename LightViewType>
    auto renderLightDepth(
        BodySetView const & bodies,
        LightViewType const & lightView,
        MonoDepthMap & target) const
        -> void;

    auto renderBody(
        Body const & body,
        glm::mat4 const & lightTransformation,
        FrustumCuller const & culler) const
        -> void;

    auto renderBodyPart(
        BodyPart const & part,
        glm::mat4 const & lightTransformation,
        FrustumCuller const & culler) const
        -> void;

    auto isVisible(
        BodyPartMesh const & mesh,
        FrustumCuller const & culler) const
        -> bool;

    auto renderMesh(BodyPartMesh const & mesh) const
        -> void;

private:

    MonoDepthShaderProgram * shader;

    ShapeDrawer const * shapeRenderer;

    bool performFrustumCulling;

};

} // namespace ape
