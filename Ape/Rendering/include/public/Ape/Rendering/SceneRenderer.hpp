#pragma once

#include <Ape/Rendering/CameraUniform.hpp>
#include <Ape/Rendering/DepthBodyRenderer.hpp>
#include <Ape/Rendering/EffectRenderer.hpp>
#include <Ape/Rendering/LightingUniform.hpp>
#include <Ape/Rendering/OffscreenSurface.hpp>
#include <Ape/Rendering/OutlinedBodyRenderer.hpp>
#include <Ape/Rendering/ShadowMapping.hpp>
#include <Ape/Rendering/ShapeRenderer.hpp>
#include <Ape/Rendering/StandardBodyRenderer.hpp>
#include <Ape/Rendering/Viewport.hpp>
#include <Ape/Rendering/WireframeBodyRenderer.hpp>

#include <Ape/GpuResource/VertexArrayObject.hpp>

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
        std::unique_ptr<ShapeRenderer> shapeRenderer,
        DepthBodyRenderer depthBodyRenderer,
        StandardBodyRenderer standardBodyRenderer,
        WireframeBodyRenderer wireframeBodyRenderer,
        OutlinedBodyRenderer outlinedBodyRenderer,
        EffectRenderer flatTextureRenderer,
        CameraSelector const & cameraSelector,
        BodySelector const & pickedBodySelector,
        Window & screenSurface,
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

    auto renderNonPickedBodies(Camera const & camera) const
        -> void;

    auto renderPickedBodies(Camera const & camera) const
        -> void;

    auto renderOffscreenSurfaceToScreen() const
        -> void;

private:

    std::unique_ptr<ShapeRenderer> shapeRenderer;

    DepthBodyRenderer depthBodyRenderer;

    StandardBodyRenderer standardBodyRenderer;

    WireframeBodyRenderer wireframeBodyRenderer;

    OutlinedBodyRenderer outlinedBodyRenderer;

    EffectRenderer flatTextureRenderer;

    CameraSelector const * cameraSelector;

    BodySelector const * pickedBodySelector;

    Window * screenSurface;

    Viewport viewport;

    ShadowMapping shadowMapping;

    OffscreenSurface offscreenSurface;

    glm::vec3 backgroundColor;

    VertexArrayObject arrayObject;

};

auto getCamera(SceneRenderer const & renderer)
    -> Camera *;

auto getScene(SceneRenderer const & renderer)
    -> Scene &;

} // namespace ape
