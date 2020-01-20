#include <Ape/Rendering/Rendering/SceneRenderer.hpp>

#include <Ape/Engine/FrameProfiling/TaskTimeProfiler.hpp>
#include <Ape/Engine/Windowing/Window.hpp>

#include <Ape/World/Model/Material.hpp>
#include <Ape/World/Model/ModelPart.hpp>
#include <Ape/World/Scene/BodySelector.hpp>
#include <Ape/World/Scene/Camera.hpp>
#include <Ape/World/Scene/CameraSelector.hpp>
#include <Ape/World/Scene/Scene.hpp>
#include <Ape/World/Shape/Shape.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>

#include <glad/glad.h>

#include <algorithm>
#include <cassert>

namespace ape
{

SceneRenderer::SceneRenderer(
    std::unique_ptr<ShapeDrawer> shapeDrawer,
    RendererSet renderers,
    CameraSelector const & cameraSelector,
    BodySelector const & pickedBodySelector,
    Window & targetSurface,
    Viewport const & viewport,
    glm::vec3 const & backgroundColor)
    : shapeDrawer{std::move(shapeDrawer)}
    , renderers{std::move(renderers)}
    , cameraSelector{&cameraSelector}
    , pickedBodySelector{&pickedBodySelector}
    , targetSurface{&targetSurface}
    , viewport{viewport}
    , shadowMapping{makeShadowMapping()}
    , offscreenSurface{targetSurface.getSize()}
    , backgroundColor{backgroundColor}
    , renderBoundingBoxes{false}
    , usePostProcessingEffects{true}
    , profiler{nullptr}
{
}

// virtual (from Renderer)
auto SceneRenderer::render()
    -> void
{
    if (!hasActiveCamera())
    {
        return;
    }

    targetSurface->makeCurrent();

    setupDrawingMode();

    if (usePostProcessingEffects)
    {
        renderSceneToOffscreenSurface();

        renderOffscreenSurfaceToScreen();
    }
    else
    {
        renderSceneToScreenSurface();
    }
}

// virtual (from Renderer)
auto SceneRenderer::getViewport() const
    -> Viewport
{
    return viewport;
}

// virtual (from Renderer)
auto SceneRenderer::setViewport(Viewport const & newViewport)
    -> void
{
    viewport = newViewport;

    offscreenSurface.setSize(viewport.size);

    shadowMapping.lightSystemView.setViewSize(viewport.size);
}

// virtual (from Renderer)
auto SceneRenderer::getCameraSelector() const
    -> CameraSelector const &
{
    return *cameraSelector;
}

// virtual (from Renderer)
auto SceneRenderer::setCameraSelector(CameraSelector const & newSelector)
    -> void
{
    cameraSelector = &newSelector;
}

// virtual (from Renderer)
auto SceneRenderer::getProfiler() const
    -> TaskTimeProfiler *
{
    return profiler;
}

// virtual (from Renderer)
auto SceneRenderer::setProfiler(TaskTimeProfiler & newProfiler)
    -> void
{
    profiler = &newProfiler;

    renderers.depthBodyRenderer.setProfiler(newProfiler);
}

auto SceneRenderer::isFrustumCullingEnabled() const
    -> bool
{
    return renderers.blinnPhongBodyRenderer.isFrustumCullingEnabled();
}

auto SceneRenderer::enableFrustumCulling(bool const enable)
    -> void
{
    renderers.blinnPhongBodyRenderer.enableFrustumCulling(enable);

    renderers.depthBodyRenderer.enableFrustumCulling(enable);
}

auto SceneRenderer::getRenderers()
    -> RendererSet &
{
    return renderers;
}

auto SceneRenderer::makeShadowMapping() const
    -> ShadowMapping
{
    auto const & lightSystem = cameraSelector->getScene().getLightSystem();

    auto const depthMapSize = basix::Size<int>{1024, 1024};

    return ShadowMapping{lightSystem, depthMapSize};
}

auto SceneRenderer::setupDrawingMode() const
    -> void
{
    auto const profiling = profiler->startTimingCpuGpuTask("Drawing mode setup", "");

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_STENCIL_TEST);

    // Helped eliminating *some* seams when bias value for omnidirectional shadow mapping in
    // Blinn-Phong shader was too low. Tweaking the value was the solution (even without enabling
    // seamless cube maps), but enabling this capability seems to make sense anyway.
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // Culling is not appropriate for all shapes. This should be done conditionally in the future.
    // However, when appropriate, it will save at least 50% of fragment shader calls.
    glEnable(GL_CULL_FACE);

    glEnable(GL_FRAMEBUFFER_SRGB);

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
    auto const profiling = profiler->startTimingCpuGpuTask("Offscreen rendering");

    // Since OpenGL 4 there always has to be at least one bound VAO when performing drawing
    // operations. Because the shape renderer may not use per-shape VAOs, we bind to a common
    // VAO here.
    with(arrayObject, [this]
    {
        shapeDrawer->beginRenderBatch();

        renderDepthMapping();

        auto const frameBufferBinder = bind(offscreenSurface.getFrameBuffer());

        renderSceneBodies();

        shapeDrawer->endRenderBatch();

        renderBodyBounds();

        renderSkybox();
    });
}

auto SceneRenderer::renderSceneToScreenSurface()
    -> void
{
    auto const profiling = profiler->startTimingCpuGpuTask("Direct rendering");

    // Since OpenGL 4 there always has to be at least one bound VAO when performing drawing
    // operations. Because the shape renderer may not use per-shape VAOs, we bind to a common
    // VAO here.
    with(arrayObject, [this]
    {
        shapeDrawer->beginRenderBatch();

        renderDepthMapping();

        targetSurface->makeCurrent();

        renderSceneBodies();

        shapeDrawer->endRenderBatch();

        renderBodyBounds();

        renderSkybox();
    });
}

auto SceneRenderer::renderDepthMapping()
    -> void
{
    auto const profiling = profiler->startTimingCpuGpuTask("Depth mapping");

    auto const & bodies = cameraSelector->getScene().getBodies();

    auto const camera = cameraSelector->getActiveCamera();

    assert(camera != nullptr);

    renderers.depthBodyRenderer.render(
        bodies,
        *camera,
        shadowMapping.lightSystemView,
        shadowMapping.depthMapping);
}

auto SceneRenderer::renderSceneBodies()
    -> void
{
    auto const profiling = profiler->startTimingCpuGpuTask("Scene body rendering");
    
    setupViewport();

    clearTargetBuffers();

    // IMPORTANT: non-picked bodies must be drawn before picked bodies in order for outlining to
    // work correctly due to the way the stencil buffer is used.

    renderNonPickedBodies();

    renderPickedBodies();
}

auto SceneRenderer::clearTargetBuffers()
    -> void
{
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

auto SceneRenderer::renderNonPickedBodies() const
    -> void
{
    auto const nonSelectedBodies = pickedBodySelector->getNonSelectedBodies();

    if (nonSelectedBodies.empty())
    {
        return;
    }

    auto const profiling = profiler->startTimingCpuGpuTask("Non-selected body rendering");

    auto const activeCamera = cameraSelector->getActiveCamera();

    assert(activeCamera != nullptr);

    auto const & lightSystem = cameraSelector->getScene().getLightSystem();

    auto const & fog = cameraSelector->getScene().getFog();

    renderers.blinnPhongBodyRenderer.render(
        nonSelectedBodies,
        *activeCamera,
        lightSystem,
        fog,
        shadowMapping);
}

auto SceneRenderer::renderPickedBodies() const
    -> void
{
    auto const selectedBodies = pickedBodySelector->getSelectedBodies();

    if (selectedBodies.empty())
    {
        return;
    }

    auto const profiling = profiler->startTimingCpuGpuTask("Selected body rendering");

    auto const activeCamera = cameraSelector->getActiveCamera();

    assert(activeCamera != nullptr);

    auto const & lightSystem = cameraSelector->getScene().getLightSystem();

    auto const & fog = cameraSelector->getScene().getFog();

    renderers.outlinedBodyRenderer.render(
        selectedBodies,
        *activeCamera,
        lightSystem,
        fog,
        shadowMapping);
}

auto SceneRenderer::renderBodyBounds() const
    -> void
{
    if (!renderBoundingBoxes)
    {
        return;
    }

    auto const profiling = profiler->startTimingCpuGpuTask("Bounding box rendering");

    auto const activeCamera = cameraSelector->getActiveCamera();

    assert(activeCamera != nullptr);

    auto const & bodies = getScene(*this).getBodies();

    renderers.boundsRenderer.render(bodies, *activeCamera);
}

auto SceneRenderer::renderSkybox() const
    -> void
{
    auto const profiling = profiler->startTimingCpuGpuTask("Skybox rendering");

    auto const activeCamera = cameraSelector->getActiveCamera();

    assert(activeCamera != nullptr);

    auto const & lightSystem = cameraSelector->getScene().getLightSystem();

    auto const & fog = cameraSelector->getScene().getFog();

    renderers.skyboxRenderer.render(*activeCamera, lightSystem, fog);
}

auto SceneRenderer::renderOffscreenSurfaceToScreen() const
    -> void
{
    auto const profiling = profiler->startTimingCpuGpuTask("Post-processing effects");

    targetSurface->makeCurrent();

    setupViewport();

    auto const & texture = offscreenSurface.getColorBuffer();

    renderers.effectRenderer.render(texture);
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
