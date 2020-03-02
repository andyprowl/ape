#pragma once

#include <Ape/Rendering/Lighting/OmniDepthMap.hpp>

#include <Ape/World/Scene/BodyRange.hpp>

#include <vector>

namespace ape
{

class Body;
class BodyPart;
class BodyPartMesh;
class Camera;
class Culler;
class DepthMapping;
class LightSystemView;
class Mesh;
class OmniDepthShaderProgram;
class PointLight;
class PointLightView;
class ShapeDrawer;
class TaskTimeProfiler;

class OmniDepthBodyRenderer
{

public:

    using BodySetView = basix::ContainerView<std::vector<Body>>;

public:

    OmniDepthBodyRenderer(
        OmniDepthShaderProgram & shader,
        ShapeDrawer & shapeRenderer);

    auto render(
        BodySetView const & bodies,
        Camera const & viewerCamera,
        LightSystemView const & lightSystemView,
        DepthMapping & target) const
        -> void;

    auto isFrustumCullingEnabled() const
        -> bool;

    auto enableFrustumCulling(bool enable)
        -> void;

    auto setProfiler(TaskTimeProfiler & newProfiler)
        -> void;

private:

    auto renderLightSetDepth(
        BodySetView const & bodies,
        Camera const & viewerCamera,
        std::vector<PointLight> const & lights,
        std::vector<PointLightView> const & lightViews,
        OmniDepthMap & depthMap) const
        -> void;

    auto renderLightDepth(
        BodySetView const & bodies,
        Camera const & viewerCamera,
        PointLightView const & lightView,
        int lightIndex,
        OmniDepthMap & target) const
        -> void;

    auto renderLightDepth(
        BodySetView const & bodies,
        Camera const & viewerCamera,
        Camera const & lightViewt) const
        -> void;

    auto renderBody(Body const & body, Camera const & lightView, Culler const & culler) const
        -> void;

    auto renderBodyPart(
        BodyPart const & part,
        Camera const & lightView,
        Culler const & culler) const
        -> void;

    auto isVisible(
        BodyPartMesh const & mesh,
        Culler const & culler) const
        -> bool;

    auto renderMesh(BodyPartMesh const & mesh) const
        -> void;

private:

    OmniDepthShaderProgram * shader;

    ShapeDrawer * shapeRenderer;

    TaskTimeProfiler * profiler;

    bool performFrustumCulling;

};

} // namespace ape
