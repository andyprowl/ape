#include <Ape/Rendering/Lighting/OmniDepthFlatBodyRenderer.hpp>

#include <Ape/Rendering/Lighting/DepthMapping.hpp>
#include <Ape/Rendering/Lighting/LightSystemView.hpp>
#include <Ape/Rendering/Lighting/OmniDepthFlatShaderProgram.hpp>

#include <Ape/Rendering/Culling/PerspectiveLightCuller.hpp>
#include <Ape/Rendering/Culling/RadarFrustumCuller.hpp>
#include <Ape/Engine/FrameProfiling/TaskTimeProfiler.hpp>

#include <Ape/World/Model/Mesh.hpp>
#include <Ape/World/Model/ModelPart.hpp>
#include <Ape/World/Scene/Body.hpp>
#include <Ape/World/Scene/BodyPart.hpp>
#include <Ape/World/Scene/Camera.hpp>
#include <Ape/World/Scene/LightSystem.hpp>
#include <Ape/World/Shape/ShapeDrawer.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>
#include <Glow/Texture/TextureCubeFace.hpp>

#include <glad/glad.h>

#include <array>

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

auto getFaceCamera(
    PointLightView::FaceCameraSet const & faceCameras,
    glow::TextureCubeFace const face)
    -> Camera const &
{
    using CameraMemberPtr = Camera PointLightView::FaceCameraSet:: *;

    static auto constexpr const cameras = std::array<CameraMemberPtr, 6u>{{
        &PointLightView::FaceCameraSet::right,
        &PointLightView::FaceCameraSet::left,
        &PointLightView::FaceCameraSet::top,
        &PointLightView::FaceCameraSet::bottom,
        &PointLightView::FaceCameraSet::front,
        &PointLightView::FaceCameraSet::back}};

    auto const cameraMemberPtr = cameras[static_cast<std::size_t>(face)];

    return faceCameras.*cameraMemberPtr;
}

} // unnamed namespace

OmniDepthFlatBodyRenderer::OmniDepthFlatBodyRenderer(
    OmniDepthFlatShaderProgram & shader,
    ShapeDrawer & shapeRenderer)
    : shader{&shader}
    , shapeRenderer{&shapeRenderer}
    , profiler{nullptr}
    , performFrustumCulling{true}
{
}

auto OmniDepthFlatBodyRenderer::render(
    BodySetView const & bodies,
    Camera const & viewerCamera,
    LightSystemView const & lightSystemView,
    DepthMapping & target) const
    -> void
{
    auto const profiling = profiler->startTimingCpuGpuTask("Omnidirectional flat shadow mapping");

    auto const shaderBinder = bind(*shader);
    
    renderLightSetDepth(
        bodies,
        viewerCamera,
        lightSystemView.getLightSystem().point,
        lightSystemView.getPointView(),
        target.getPointMapping());
}

auto OmniDepthFlatBodyRenderer::isFrustumCullingEnabled() const
    -> bool
{
    return performFrustumCulling;
}

auto OmniDepthFlatBodyRenderer::enableFrustumCulling(bool const enable)
    -> void
{
    performFrustumCulling = enable;
}

auto OmniDepthFlatBodyRenderer::setProfiler(TaskTimeProfiler & newProfiler)
    -> void
{
    profiler = &newProfiler;
}

auto OmniDepthFlatBodyRenderer::renderLightSetDepth(
    BodySetView const & bodies,
    Camera const & viewerCamera,
    std::vector<PointLight> const & lights,
    std::vector<PointLightView> const & lightViews,
    std::vector<OmniDepthMap> & depthMaps) const
    -> void
{
    for (auto i = 0u; i < lights.size(); ++i)
    {
        if ((not lights[i].isTurnedOn()) || (not lights[i].isCastingShadow()))
        {
            continue;
        }

        shader->lightPosition.set(lights[i].getPosition());

        auto const & lightView = lightViews[i];

        auto & depthMap = depthMaps[i];

        renderLightDepth(bodies, viewerCamera, lightView, depthMap);
    }
}

auto OmniDepthFlatBodyRenderer::renderLightDepth(
    BodySetView const & bodies,
    Camera const & viewerCamera,
    PointLightView const & lightView,
    OmniDepthMap & target) const
    -> void
{
    auto & frameBuffer = target.getFrameBuffer();

    auto & depthTexture = target.getTexture();

    auto const & lightCameras = lightView.getFaceCameras();

    auto const binder = bind(frameBuffer);

    for (auto const face : glow::getCubeTextureFaces())
    {
        frameBuffer.attach(depthTexture, face, glow::FrameBufferAttachment::depth);

        auto const & faceCamera = getFaceCamera(lightCameras, face);

        renderLightDepth(bodies, viewerCamera, faceCamera);
    }
}

auto OmniDepthFlatBodyRenderer::renderLightDepth(
    BodySetView const & bodies,
    Camera const & viewerCamera,
    Camera const & lightView) const
    -> void
{
    // TODO: benchmark performance benefit of PerspectiveLightCuller by temporarily replacing it
    // with a RadarFrustumCuller
    //(void)viewerCamera;
    //auto const culler = RadarFrustumCuller{lightView.getCamera()};

    auto const culler = PerspectiveLightCuller{lightView, viewerCamera};

    if (culler.isCullingVolumeEmpty())
    {
        return;
    }

    glClear(GL_DEPTH_BUFFER_BIT);

    for (auto const & body : bodies)
    {
        renderBody(asReference(body), lightView, culler);
    }
}

auto OmniDepthFlatBodyRenderer::renderBody(
    Body const & body,
    Camera const & lightView,
    Culler const & culler) const
    -> void
{
    if (not body.isCastingShadow())
    {
        return;
    }

    for (auto const & part : body.getParts())
    {
        renderBodyPart(part, lightView, culler);
    }
}

auto OmniDepthFlatBodyRenderer::renderBodyPart(
    BodyPart const & part,
    Camera const & lightView,
    Culler const & culler) const
    -> void
{
    auto const & worldTransformation = part.getWorldTransformation();
    
    shader->worldTransformation.set(worldTransformation);

    auto const & lightTransformation = lightView.getTransformation();

    shader->lightTransformation.set(lightTransformation * worldTransformation);

    for (auto const & bodyMesh : part.getMeshes())
    {
        if (not isVisible(bodyMesh, culler))
        {
            continue;
        }

        renderMesh(bodyMesh);
    }
}

auto OmniDepthFlatBodyRenderer::isVisible(
    BodyPartMesh const & mesh,
    Culler const & culler) const
    -> bool
{
    if (not isFrustumCullingEnabled())
    {
        return true;
    }

    auto const & boundingSphere = mesh.getBoundingVolumes().getSphere();

    auto const relation = culler.isSphereContained(boundingSphere);

    return (relation != ContainmentRelation::fullyOutside);
}

auto OmniDepthFlatBodyRenderer::renderMesh(BodyPartMesh const & mesh) const
    -> void
{
    auto const & meshModel = mesh.getModel();

    auto const & shape = meshModel.getShape();
    
    shapeRenderer->render(shape);
}

} // namespace ape
