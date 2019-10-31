#include <Ape/Rendering/SceneRenderer.hpp>

#include <Ape/GpuResource/ScopedBinder.hpp>
#include <Ape/Model/Material.hpp>
#include <Ape/Model/ModelPart.hpp>
#include <Ape/Model/Shape.hpp>
#include <Ape/Scene/BodySelector.hpp>
#include <Ape/Scene/Camera.hpp>
#include <Ape/Scene/CameraSelector.hpp>
#include <Ape/Scene/Scene.hpp>
#include <Ape/Windowing/Window.hpp>

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
    EffectRenderer effectRenderer,
    CameraSelector const & cameraSelector,
    BodySelector const & pickedBodySelector,
    Window & targetSurface,
    Viewport const & viewport,
    glm::vec3 const & backgroundColor)
    : shapeRenderer{std::move(shapeRenderer)}
    , depthBodyRenderer{std::move(depthBodyRenderer)}
    , standardBodyRenderer{std::move(standardBodyRenderer)}
    , wireframeBodyRenderer{std::move(wireframeBodyRenderer)}
    , outlinedBodyRenderer{std::move(outlinedBodyRenderer)}
    , effectRenderer{std::move(effectRenderer)}
    , cameraSelector{&cameraSelector}
    , pickedBodySelector{&pickedBodySelector}
    , targetSurface{&targetSurface}
    , viewport{viewport}
    , shadowMapping{makeShadowMapping()}
    , offscreenSurface{targetSurface.getSize()}
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
    setupDrawingMode();

    setupViewport();

    if (hasActiveCamera())
    {
        renderSceneToOffscreenSurface();
    }

    renderOffscreenSurfaceToScreen();
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

    wireframeBodyRenderer.setViewport(viewport);

    effectRenderer.setViewport(viewport);

    offscreenSurface.setSize(viewport.size);

    shadowMapping.lightingView.setViewSize(viewport.size);
}

auto SceneRenderer::makeShadowMapping() const
    -> ShadowMapping
{
    auto const & lighting = cameraSelector->getScene().getLighting();

    auto const depthMapSize = Size<int>{1024, 1024};

    return ShadowMapping{lighting, depthMapSize};
}

auto SceneRenderer::setupDrawingMode() const
    -> void
{
    glCullFace(GL_BACK);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

auto SceneRenderer::setupViewport() const
    -> void
{
    glViewport(viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);
}

auto SceneRenderer::hasActiveCamera() const
    -> bool
{
    auto const activeCamera = cameraSelector->getActiveCamera();

    return (activeCamera != nullptr);
}

auto SceneRenderer::renderSceneToOffscreenSurface()
    -> void
{
    with(arrayObject, [this]
    {
        renderDepthMapping();

        renderSceneBodiesToOffscreenSurface();
    });
}

auto SceneRenderer::renderDepthMapping()
    -> void
{
    auto const & bodies = cameraSelector->getScene().getBodies();

    depthBodyRenderer.render(bodies, shadowMapping.lightingView, shadowMapping.depthMapping);
}

auto SceneRenderer::renderSceneBodiesToOffscreenSurface()
    -> void
{
    auto const frameBufferBinder = bind(offscreenSurface.getFrameBuffer());

    clearTargetBuffers();

    auto const activeCamera = cameraSelector->getActiveCamera();

    assert(activeCamera != nullptr);

    // IMPORTANT: non-picked bodies must be drawn before picked bodies in order for outlining to
    // work correctly due to the way the stencil buffer is used.

    renderNonPickedBodies(*activeCamera);

    renderPickedBodies(*activeCamera);
}

auto SceneRenderer::clearTargetBuffers()
    -> void
{
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

auto SceneRenderer::renderNonPickedBodies(Camera const & camera) const
    -> void
{
    auto const nonSelectedBodies = pickedBodySelector->getNonSelectedBodies();

    if (nonSelectedBodies.empty())
    {
        return;
    }

    auto const & lighting = cameraSelector->getScene().getLighting();

    standardBodyRenderer.render(nonSelectedBodies, camera, lighting, shadowMapping);
}

auto SceneRenderer::renderPickedBodies(Camera const & camera) const
    -> void
{
    auto const selectedBodies = pickedBodySelector->getSelectedBodies();

    if (selectedBodies.empty())
    {
        return;
    }

    auto const & lighting = cameraSelector->getScene().getLighting();

    outlinedBodyRenderer.render(selectedBodies, camera, lighting, shadowMapping);
}

auto SceneRenderer::renderOffscreenSurfaceToScreen() const
    -> void
{
    targetSurface->makeCurrent();

    auto const & texture = offscreenSurface.getColorBuffer();

    effectRenderer.render(texture);
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
