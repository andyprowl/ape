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
    StandardBodyRenderer standardBodyRenderer,
    WireframeBodyRenderer wireframeBodyRenderer,
    OutlinedBodyRenderer outlinedBodyRenderer,
    CameraSelector const & cameraSelector,
    BodySelector const & pickedBodySelector,
    glm::vec3 const & backgroundColor)
    : shapeRenderer{std::move(shapeRenderer)}
    , standardBodyRenderer{std::move(standardBodyRenderer)}
    , wireframeBodyRenderer{std::move(wireframeBodyRenderer)}
    , outlinedBodyRenderer{std::move(outlinedBodyRenderer)}
    , cameraSelector{&cameraSelector}
    , pickedBodySelector{&pickedBodySelector}
    , viewport{{0, 0}, {0, 0}}
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

    auto const & lighting = cameraSelector->getScene().getLighting();

    renderNonPickedBodies(*activeCamera, lighting);

    renderPickedBodies(*activeCamera, lighting);
}

auto SceneRenderer::renderNonPickedBodies(Camera const & camera, Lighting const & lighting) const
    -> void
{
    auto const nonSelectedBodies = pickedBodySelector->getNonSelectedBodies();

    if (nonSelectedBodies.empty())
    {
        return;
    }

    standardBodyRenderer.render(nonSelectedBodies, camera, lighting);
}

auto SceneRenderer::renderPickedBodies(Camera const & camera, Lighting const & lighting) const
    -> void
{
    auto const selectedBodies = pickedBodySelector->getSelectedBodies();

    if (selectedBodies.empty())
    {
        return;
    }

    outlinedBodyRenderer.render(selectedBodies, camera, lighting);

    standardBodyRenderer.render(selectedBodies, camera, lighting);
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
