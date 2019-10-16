#pragma once

#include <Rendering/CameraUniform.hpp>
#include <Rendering/DepthBodyRenderer.hpp>
#include <Rendering/LightingUniform.hpp>
#include <Rendering/OutlinedBodyRenderer.hpp>
#include <Rendering/ShadowMapping.hpp>
#include <Rendering/ShapeRenderer.hpp>
#include <Rendering/StandardBodyRenderer.hpp>
#include <Rendering/Viewport.hpp>
#include <Rendering/WireframeBodyRenderer.hpp>

#include <GpuResource/VertexArrayObject.hpp>

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
        CameraSelector const & cameraSelector,
        BodySelector const & pickedBodySelector,
        Window & surface,
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

    auto hasActiveCamera() const
        -> bool;

    auto clear() const
        -> void;

    auto renderDepthMapping()
        -> void;

    auto renderSceneBodies()
        -> void;

    auto renderNonPickedBodies(Camera const & camera) const
        -> void;

    auto renderPickedBodies(Camera const & camera) const
        -> void;

private:

    std::unique_ptr<ShapeRenderer> shapeRenderer;

    DepthBodyRenderer depthBodyRenderer;

    StandardBodyRenderer standardBodyRenderer;

    WireframeBodyRenderer wireframeBodyRenderer;

    OutlinedBodyRenderer outlinedBodyRenderer;

    CameraSelector const * cameraSelector;

    BodySelector const * pickedBodySelector;

    Window * surface;

    Viewport viewport;

    ShadowMapping shadowMapping;

    glm::vec3 backgroundColor;

    VertexArrayObject arrayObject;

};

auto getCamera(SceneRenderer const & renderer)
    -> Camera *;

auto getScene(SceneRenderer const & renderer)
    -> Scene &;

} // namespace ape
