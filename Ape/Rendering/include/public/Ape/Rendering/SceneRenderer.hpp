#pragma once

#include <Ape/Rendering/OffscreenSurface.hpp>
#include <Ape/Rendering/OutlinedBodyRenderer.hpp>
#include <Ape/Rendering/Viewport.hpp>

#include <Ape/Effect/EffectRenderer.hpp>
#include <Ape/Lighting/DepthBodyRenderer.hpp>
#include <Ape/Lighting/ShadowMapping.hpp>
#include <Ape/Lighting/LightingBodyRenderer.hpp>
#include <Ape/Shape/ShapeDrawer.hpp>
#include <Ape/Skybox/SkyboxRenderer.hpp>
#include <Ape/Wireframe/WireframeBodyRenderer.hpp>

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

    SceneRenderer(
        std::unique_ptr<ShapeDrawer> shapeRenderer,
        DepthBodyRenderer depthBodyRenderer,
        LightingBodyRenderer standardBodyRenderer,
        WireframeBodyRenderer wireframeBodyRenderer,
        OutlinedBodyRenderer outlinedBodyRenderer,
        SkyboxRenderer skyboxRenderer,
        EffectRenderer effectRenderer,
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

    auto renderSkybox() const
        -> void;

    auto renderOffscreenSurfaceToScreen() const
        -> void;

private:

    std::unique_ptr<ShapeDrawer> shapeRenderer;

    DepthBodyRenderer depthBodyRenderer;

    LightingBodyRenderer standardBodyRenderer;

    WireframeBodyRenderer wireframeBodyRenderer;

    OutlinedBodyRenderer outlinedBodyRenderer;

    SkyboxRenderer skyboxRenderer;

    EffectRenderer effectRenderer;

    CameraSelector const * cameraSelector;

    BodySelector const * pickedBodySelector;

    Window * targetSurface;

    Viewport viewport;

    ShadowMapping shadowMapping;

    OffscreenSurface offscreenSurface;

    glm::vec3 backgroundColor;

    glow::VertexArrayObject arrayObject;

};

auto getCamera(SceneRenderer const & renderer)
    -> Camera *;

auto getScene(SceneRenderer const & renderer)
    -> Scene &;

} // namespace ape
