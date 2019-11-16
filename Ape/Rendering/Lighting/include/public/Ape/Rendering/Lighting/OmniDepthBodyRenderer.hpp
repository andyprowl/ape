#pragma once

#include <Ape/Rendering/Lighting/OmniDepthMap.hpp>

#include <Ape/World/Scene/BodyRange.hpp>

#include <vector>

namespace ape
{

class Body;
class BodyPart;
class Camera;
class DepthMapping;
class LightSystemView;
class Mesh;
class OmniDepthShaderProgram;
class PointLight;
class PointLightView;
class ShapeDrawer;

class OmniDepthBodyRenderer
{

public:

    using BodySetView = basix::ContainerView<std::vector<Body>>;

public:

    OmniDepthBodyRenderer(OmniDepthShaderProgram & shader, ShapeDrawer const & shapeRenderer);

    auto render(
        BodySetView const & bodies,
        LightSystemView const & lightSystemView,
        DepthMapping & target) const
        -> void;

private:

    auto renderLightSetDepth(
        BodySetView const & bodies,
        std::vector<PointLight> const & lights,
        std::vector<PointLightView> const & lightViews,
        std::vector<OmniDepthMap> & depthMaps) const
        -> void;

    auto renderLightDepth(
        BodySetView const & bodies,
        PointLightView const & lightView,
        OmniDepthMap & target) const
        -> void;

    auto renderBody(Body const & body, PointLightView const & lightView) const
        -> void;

    auto renderBodyPart(BodyPart const & part, PointLightView const & lightView) const
        -> void;

    auto renderMesh(Mesh const & mesh) const
        -> void;

private:

    OmniDepthShaderProgram * shader;

    ShapeDrawer const * shapeRenderer;

};

} // namespace ape
