#include <Rendering/SceneRenderer.hpp>

#include <Scene/BodySelector.hpp>
#include <Scene/Camera.hpp>
#include <Scene/CameraSelector.hpp>
#include <Scene/Scene.hpp>

#include <Model/Material.hpp>
#include <Model/ModelPart.hpp>
#include <Model/Shape.hpp>

#include <GpuResource/ScopedBinder.hpp>

#include <glad/glad.h>

#include <algorithm>
#include <cassert>

namespace ape
{

SceneRenderer::SceneRenderer(
    std::unique_ptr<ShapeRenderer> shapeRenderer,
    DepthBodyRenderer depthBodyRenderer,
    StandardBodyRenderer standardBodyRenderer,
    WireframeBodyRenderer wireframeBodyRenderer,
    OutlinedBodyRenderer outlinedBodyRenderer,
    CameraSelector const & cameraSelector,
    BodySelector const & pickedBodySelector,
    Viewport const & viewport,
    glm::vec3 const & backgroundColor)
    : shapeRenderer{std::move(shapeRenderer)}
    , depthBodyRenderer{std::move(depthBodyRenderer)}
    , standardBodyRenderer{std::move(standardBodyRenderer)}
    , wireframeBodyRenderer{std::move(wireframeBodyRenderer)}
    , outlinedBodyRenderer{std::move(outlinedBodyRenderer)}
    , cameraSelector{&cameraSelector}
    , pickedBodySelector{&pickedBodySelector}
    , viewport{viewport}
    , lightingView{cameraSelector.getScene().getLighting(), viewport.size}
    , backgroundColor{backgroundColor}
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_STENCIL_TEST);

    // Culling is not appropriate for all shapes. This should be done conditionally in the future.
    // However, when appropriate, it will save at least 50% of fragment shader calls.
    glEnable(GL_CULL_FACE);
}

auto SceneRenderer::render()
    -> void
{
    clear();

    if (!hasActiveCamera())
    {
        return;
    }

    renderBodies();
}

auto SceneRenderer::getCameraSelector() const
    -> CameraSelector const &
{
    return *cameraSelector;
}

auto SceneRenderer::setCameraSelector(CameraSelector const & newSelector)
    -> void
{
    cameraSelector = &newSelector;
}

auto SceneRenderer::getViewport() const
    -> Viewport
{
    return viewport;
}

auto SceneRenderer::setViewport(Viewport const & newViewport)
    -> void
{
    viewport = newViewport;

    standardBodyRenderer.setViewport(viewport);

    outlinedBodyRenderer.setViewport(viewport);

    lightingView.setViewSize(viewport.size);
}

auto SceneRenderer::clear() const
    -> void
{
    glCullFace(GL_BACK);

    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glViewport(viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);
}

auto SceneRenderer::hasActiveCamera() const
    -> bool
{
    auto const activeCamera = cameraSelector->getActiveCamera();

    return (activeCamera != nullptr);
}

auto SceneRenderer::renderBodies() const
    -> void
{
    auto const arrayObjectBinder = ScopedBinder{arrayObject};

    auto const activeCamera = cameraSelector->getActiveCamera();

    assert(activeCamera != nullptr);

    auto const & lightView = lightingView.getSpotView().at(0);

    renderDepthMap(lightView);

    renderNonPickedBodies(*activeCamera, lightView);

    renderPickedBodies(*activeCamera, lightView);
}

auto SceneRenderer::renderDepthMap(Camera const & ligthView) const
    -> void
{
    auto const & bodies = cameraSelector->getScene().getBodies();

    depthBodyRenderer.render(bodies, ligthView);
}

auto SceneRenderer::renderNonPickedBodies(
    Camera const & camera,
    Camera const & lightView) const
    -> void
{
    auto const nonSelectedBodies = pickedBodySelector->getNonSelectedBodies();

    if (nonSelectedBodies.empty())
    {
        return;
    }

    auto const & lighting = cameraSelector->getScene().getLighting();

    auto const & depthMap = depthBodyRenderer.getDepthMap();

    standardBodyRenderer.render(nonSelectedBodies, camera, lighting, depthMap, lightView);
}

auto SceneRenderer::renderPickedBodies(
    Camera const & camera,
    Camera const & lightView) const
    -> void
{
    auto const selectedBodies = pickedBodySelector->getSelectedBodies();

    if (selectedBodies.empty())
    {
        return;
    }

    auto const & lighting = cameraSelector->getScene().getLighting();

    auto const & depthMap = depthBodyRenderer.getDepthMap();

    outlinedBodyRenderer.render(selectedBodies, camera, lighting, depthMap, lightView);

    standardBodyRenderer.render(selectedBodies, camera, lighting, depthMap, lightView);
}

auto getCamera(SceneRenderer const & renderer)
    -> Camera *
{
    return renderer.getCameraSelector().getActiveCamera();
}

auto getScene(SceneRenderer const & renderer)
    -> Scene &
{
    return renderer.getCameraSelector().getScene();
}

} // namespace ape
