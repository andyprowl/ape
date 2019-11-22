#pragma once

#include <Ape/Rendering/Rendering/BodyBoundsRenderer.hpp>
#include <Ape/Rendering/Rendering/OffscreenSurface.hpp>
#include <Ape/Rendering/Rendering/OutlinedBodyRenderer.hpp>
#include <Ape/Rendering/Rendering/Viewport.hpp>

#include <Ape/Rendering/Effect/EffectRenderer.hpp>
#include <Ape/Rendering/Lighting/DepthBodyRenderer.hpp>
#include <Ape/Rendering/Lighting/ShadowMapping.hpp>
#include <Ape/Rendering/Lighting/BlinnPhongBodyRenderer.hpp>
#include <Ape/Rendering/Skybox/SkyboxRenderer.hpp>
#include <Ape/Rendering/Wireframe/WireframeBodyRenderer.hpp>

#include <Ape/World/Shape/ShapeDrawer.hpp>

#include <Glow/BufferObject/VertexArrayObject.hpp>

#include <memory>
#include <stdexcept>

namespace ape
{

class Body;
class BodyPart;
class BodySelector;
class Camera;
class CameraSelector;
class Scene;
class Window;

class SceneRenderer
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
        std::unique_ptr<ShapeDrawer> shapeDrawer,
        RendererSet renderers,
        CameraSelector const & cameraSelector,
        BodySelector const & pickedBodySelector,
        Window & targetSurface,
        Viewport const & viewport,
        glm::vec3 const & backgroundColor);

    auto render()
        -> void;

    auto getCameraSelector() const
        -> CameraSelector const &;

    auto setCameraSelector(CameraSelector const & newSelector)
        -> void;
    
    auto getViewport() const
        -> Viewport;

    auto setViewport(Viewport const & newViewport)
        -> void;

    auto isFrustumCullingEnabled() const
        -> bool;

    auto enableFrustumCulling(bool enable)
        -> void;

private:

    auto makeShadowMapping() const
        -> ShadowMapping;

    auto setupDrawingMode() const
        -> void;

    auto setupViewport() const
        -> void;

    auto hasActiveCamera() const
        -> bool;

    auto renderSceneToOffscreenSurface()
        -> void;

    auto renderDepthMapping()
        -> void;

    auto renderSceneBodiesToOffscreenSurface()
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

    std::unique_ptr<ShapeDrawer> shapeDrawer;

    RendererSet renderers;

    CameraSelector const * cameraSelector;

    BodySelector const * pickedBodySelector;

    Window * targetSurface;

    Viewport viewport;

    ShadowMapping shadowMapping;

    OffscreenSurface offscreenSurface;

    glm::vec3 backgroundColor;

    bool renderBoundingBoxes;

    glow::VertexArrayObject arrayObject;

};

auto getCamera(SceneRenderer const & renderer)
    -> Camera *;

auto getScene(SceneRenderer const & renderer)
    -> Scene &;

} // namespace ape
