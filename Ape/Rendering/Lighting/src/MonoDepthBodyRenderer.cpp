#include <Ape/Rendering/Lighting/MonoDepthBodyRenderer.hpp>

#include <Ape/Rendering/Lighting/DepthMapping.hpp>
#include <Ape/Rendering/Lighting/LightSystemView.hpp>
#include <Ape/Rendering/Lighting/MonoDepthShaderProgram.hpp>

#include <Ape/Rendering/Culling/RadarFrustumCuller.hpp>

#include <Ape/World/Model/Mesh.hpp>
#include <Ape/World/Model/ModelPart.hpp>
#include <Ape/World/Scene/Body.hpp>
#include <Ape/World/Scene/BodyPart.hpp>
#include <Ape/World/Scene/Camera.hpp>
#include <Ape/World/Scene/LightSystem.hpp>
#include <Ape/World/Shape/ShapeDrawer.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>

#include <glad/glad.h>

namespace ape
{

namespace
{

auto asReference(Body const & body)
    -> Body const &
{
    return body;
}

auto asReference(Body * const body)
    -> Body const &
{
    return *body;
}

} // unnamed namespace

MonoDepthBodyRenderer::MonoDepthBodyRenderer(
    MonoDepthShaderProgram & shader,
    ShapeDrawer const & shapeRenderer)
    : shader{&shader}
    , shapeRenderer{&shapeRenderer}
    , performFrustumCulling{true}
{
}

auto MonoDepthBodyRenderer::render(
    BodySetView const & bodies,
    LightSystemView const & lightSystemView,
    DepthMapping & target) const
    -> void
{
    auto const shaderBinder = bind(*shader);

    renderSpotLightSetDepth(bodies, lightSystemView, target);

    renderDirectionalLightSetDepth(bodies, lightSystemView, target);
}

auto MonoDepthBodyRenderer::isFrustumCullingEnabled() const
    -> bool
{
    return performFrustumCulling;
}

auto MonoDepthBodyRenderer::enableFrustumCulling(bool const enable)
    -> void
{
    performFrustumCulling = enable;
}

auto MonoDepthBodyRenderer::renderSpotLightSetDepth(
    BodySetView const & bodies,
    LightSystemView const & lightSystemView,
    DepthMapping & target) const
    -> void
{
    auto const & lightSystem = lightSystemView.getLighting();

    auto const & spotView = lightSystemView.getSpotView();

    auto & spotMapping = target.getSpotMapping();

    renderLightSetDepth(bodies, lightSystem.spot, spotView, spotMapping);
}

auto MonoDepthBodyRenderer::renderDirectionalLightSetDepth(
    BodySetView const & bodies,
    LightSystemView const & lightSystemView,
    DepthMapping & target) const
    -> void
{
    auto const & lightSystem = lightSystemView.getLighting();

    auto const & directionalView = lightSystemView.getDirectionalView();

    auto & directionalMapping = target.getDirectionalMapping();

    renderLightSetDepth(bodies, lightSystem.directional, directionalView, directionalMapping);
}

template<typename LightType, typename LightViewType>
auto MonoDepthBodyRenderer::renderLightSetDepth(
    BodySetView const & bodies,
    std::vector<LightType> const & lights,
    std::vector<LightViewType> const & lightViews,
    std::vector<MonoDepthMap> & depthMaps) const
    -> void
{
    for (auto i = 0u; i < lights.size(); ++i)
    {
        if (!lights[i].isTurnedOn())
        {
            continue;
        }

        auto const & lightView = lightViews[i];

        auto & depthMap = depthMaps[i];

        renderLightDepth(bodies, lightView, depthMap);
    }
}

template<typename LightViewType>
auto MonoDepthBodyRenderer::renderLightDepth(
    BodySetView const & bodies,
    LightViewType const & lightView,
    MonoDepthMap & target) const
    -> void
{
    auto const binder = bind(target.getFrameBuffer());

    auto const mapSize = target.getSize();

    glViewport(0, 0, mapSize.width, mapSize.height);

    glClear(GL_DEPTH_BUFFER_BIT);

    auto const culler = RadarFrustumCuller{lightView.getCamera()};

    for (auto const & body : bodies)
    {
        auto const & lightTransformation = ape::getTransformation(lightView);

        renderBody(asReference(body), lightTransformation, culler);
    }
}

auto MonoDepthBodyRenderer::renderBody(
    Body const & body,
    glm::mat4 const & lightTransformation,
    RadarFrustumCuller const & culler) const
    -> void
{
    for (auto const & part : body.getParts())
    {
        renderBodyPart(part, lightTransformation, culler);
    }
}

auto MonoDepthBodyRenderer::renderBodyPart(
    BodyPart const & part,
    glm::mat4 const & lightTransformation,
    RadarFrustumCuller const & culler) const
    -> void
{
    auto const & worldTransformation = part.getWorldTransformation();
    
    shader->lightTransformation = lightTransformation * worldTransformation;

    for (auto const bodyMesh : part.getMeshes())
    {
        if (!isVisible(bodyMesh, culler))
        {
            continue;
        }

        renderMesh(bodyMesh);
    }
}

auto MonoDepthBodyRenderer::isVisible(
    BodyPartMesh const & mesh,
    RadarFrustumCuller const & culler) const
    -> bool
{
    if (!isFrustumCullingEnabled())
    {
        return true;
    }

    auto const & boundingSphere = mesh.getBoundingVolumes().getSphere();

    auto const relation = culler.computeFrustumRelation(boundingSphere);

    return (relation != ContainmentRelation::fullyOutside);
}

auto MonoDepthBodyRenderer::renderMesh(BodyPartMesh const & mesh) const
    -> void
{
    auto const & meshModel = mesh.getModel();

    auto const & shape = meshModel.getShape();
    
    shapeRenderer->render(shape);
}

} // namespace ape
