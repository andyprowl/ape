#pragma once

#include <Ape/Rendering/OmniDepthMap.hpp>

#include <Ape/Scene/BodyRange.hpp>

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
class ShapeRenderer;

class OmniDepthBodyRenderer
{

public:

    using BodySetView = ContainerView<std::vector<Body>>;

public:

    OmniDepthBodyRenderer(OmniDepthShaderProgram & shader, ShapeRenderer const & shapeRenderer);

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
        PointLightView const & lightTransformation,
        OmniDepthMap & target) const
        -> void;

    auto renderBody(Body const & body, PointLightView const & lightTransformation) const
        -> void;

    auto renderBodyPart(BodyPart const & part, PointLightView const & lightTransformation) const
        -> void;

    auto renderMesh(Mesh const & mesh) const
        -> void;

private:

    OmniDepthShaderProgram * shader;

    ShapeRenderer const * shapeRenderer;

};

} // namespace ape
