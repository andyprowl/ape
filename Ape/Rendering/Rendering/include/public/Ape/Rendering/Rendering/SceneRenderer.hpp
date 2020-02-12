#pragma once

#include <Ape/Rendering/Rendering/BodyBoundsRenderer.hpp>
#include <Ape/Rendering/Rendering/OffscreenSurface.hpp>
#include <Ape/Rendering/Rendering/OutlinedBodyRenderer.hpp>

#include <Ape/Rendering/Effect/EffectRenderer.hpp>
#include <Ape/Rendering/Lighting/DepthBodyRenderer.hpp>
#include <Ape/Rendering/Lighting/BlinnPhongBodyRenderer.hpp>
#include <Ape/Rendering/Lighting/ShadowMapping.hpp>
#include <Ape/Rendering/Skybox/SkyboxRenderer.hpp>
#include <Ape/Rendering/Wireframe/WireframeBodyRenderer.hpp>

#include <Ape/Engine/Engine/Renderer.hpp>
#include <Ape/Engine/Windowing/Viewport.hpp>
#include <Ape/World/Shape/ShapeDrawer.hpp>

#include <Glow/BufferObject/VertexArrayObject.hpp>

#include <memory>
#include <stdexcept>
#include <string_view>

namespace ape
{

class Body;
class BodyPart;
class BodySelector;
class Camera;
class CameraSelector;
class Scene;
class TaskTimeProfiler;
class Window;

class SceneRenderer : public Renderer
{

public:

    class RendererSet
    {

    public:

        RendererSet(
            DepthBodyRenderer depthBodyRenderer,
            BlinnPhongBodyRenderer blinnPhongBodyRenderer,
            WireframeBodyRenderer wireframeBodyRenderer,
            OutlinedBodyRenderer outlinedBodyRenderer,
            BodyBoundsRenderer boundsRenderer,
            SkyboxRenderer skyboxRenderer,
            EffectRenderer effectRenderer)
            : depthBodyRenderer{std::move(depthBodyRenderer)}
            , blinnPhongBodyRenderer{std::move(blinnPhongBodyRenderer)}
            , wireframeBodyRenderer{std::move(wireframeBodyRenderer)}
            , outlinedBodyRenderer{std::move(outlinedBodyRenderer)}
            , boundsRenderer{std::move(boundsRenderer)}
            , skyboxRenderer{std::move(skyboxRenderer)}
            , effectRenderer{std::move(effectRenderer)}
        {
        }

    public:

        DepthBodyRenderer depthBodyRenderer;

        BlinnPhongBodyRenderer blinnPhongBodyRenderer;

        WireframeBodyRenderer wireframeBodyRenderer;

        OutlinedBodyRenderer outlinedBodyRenderer;

        BodyBoundsRenderer boundsRenderer;

        SkyboxRenderer skyboxRenderer;

        EffectRenderer effectRenderer;

    };

public:

    SceneRenderer(
        RendererSet renderers,
        std::unique_ptr<ShapeDrawer> shapeDrawer,
        std::unique_ptr<ShadowMapping> shadowMapping,
        CameraSelector const & cameraSelector,
        BodySelector const & pickedBodySelector,
        Window & targetSurface,
        Viewport const & viewport,
        glm::vec3 const & backgroundColor);

    // virtual (from Renderer)
    auto render()
        -> void override;

    // virtual (from Renderer)
    auto getViewport() const
        -> Viewport;

    // virtual (from Renderer)
    auto setViewport(Viewport const & newViewport)
        -> void;

    // virtual (from Renderer)
    auto getCameraSelector() const
        -> CameraSelector const & override;

    // virtual (from Renderer)
    auto setCameraSelector(CameraSelector const & newSelector)
        -> void;

    // virtual (from Renderer)
    auto getProfiler() const
        -> TaskTimeProfiler *;

    // virtual (from Renderer)
    auto setProfiler(TaskTimeProfiler & newProfiler)
        -> void;

    auto isFrustumCullingEnabled() const
        -> bool;

    auto enableFrustumCulling(bool enable)
        -> void;

    auto getRenderers()
        -> RendererSet &;

private:

    auto setupDrawingMode() const
        -> void;

    auto setupViewport() const
        -> void;

    auto hasActiveCamera() const
        -> bool;

    auto renderSceneToOffscreenSurface()
        -> void;
    
    auto renderSceneToScreenSurface()
        -> void;

    auto renderDepthMapping()
        -> void;

    auto renderSceneBodies()
        -> void;

    auto clearTargetBuffers()
        -> void;

    auto renderNonPickedBodies() const
        -> void;

    auto renderPickedBodies() const
        -> void;

    auto renderBodyBounds() const
        -> void;

    auto renderSkybox() const
        -> void;

    auto renderOffscreenSurfaceToScreen() const
        -> void;

private:

    RendererSet renderers;

    std::unique_ptr<ShapeDrawer> shapeDrawer;

    std::unique_ptr<ShadowMapping> shadowMapping;

    CameraSelector const * cameraSelector;

    BodySelector const * pickedBodySelector;

    Window * targetSurface;

    Viewport viewport;

    OffscreenSurface offscreenSurface;

    glm::vec3 backgroundColor;

    bool renderBoundingBoxes;

    bool usePostProcessingEffects;

    glow::VertexArrayObject arrayObject;

    TaskTimeProfiler * profiler;

};

auto getCamera(SceneRenderer const & renderer)
    -> Camera *;

auto getScene(SceneRenderer const & renderer)
    -> Scene &;

} // namespace ape
