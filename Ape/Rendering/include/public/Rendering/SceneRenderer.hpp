#pragma once

#include <Rendering/CameraUniform.hpp>
#include <Rendering/LightingUniform.hpp>
#include <Rendering/OutlinedBodyRenderer.hpp>
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

class SceneRenderer
{

public:

    SceneRenderer(
        std::unique_ptr<ShapeRenderer> shapeRenderer,
        StandardBodyRenderer standardBodyRenderer,
        WireframeBodyRenderer wireframeBodyRenderer,
        OutlinedBodyRenderer outlinedBodyRenderer,
        CameraSelector const & cameraSelector,
        BodySelector const & pickedBodySelector,
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

    auto hasActiveCamera() const
        -> bool;

    auto clear() const
        -> void;

    auto renderBodies() const
        -> void;

    auto renderNonPickedBodies(Camera const & camera, Lighting const & lighting) const
        -> void;

    auto renderPickedBodies(Camera const & camera, Lighting const & lighting) const
        -> void;

private:

    std::unique_ptr<ShapeRenderer> shapeRenderer;

    StandardBodyRenderer standardBodyRenderer;

    WireframeBodyRenderer wireframeBodyRenderer;

    OutlinedBodyRenderer outlinedBodyRenderer;

    CameraSelector const * cameraSelector;

    BodySelector const * pickedBodySelector;

    Viewport viewport;

    glm::vec3 backgroundColor;

    VertexArrayObject arrayObject;

};

auto getCamera(SceneRenderer const & renderer)
    -> Camera *;

auto getScene(SceneRenderer const & renderer)
    -> Scene &;

} // namespace ape
