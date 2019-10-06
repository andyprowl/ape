#pragma once

#include <Rendering/CameraUniform.hpp>
#include <Rendering/LightingUniform.hpp>
#include <Rendering/OutlinedBodyRenderer.hpp>
#include <Rendering/StandardBodyRenderer.hpp>
#include <Rendering/WireframeBodyRenderer.hpp>

#include <GpuResource/VertexArrayObject.hpp>

#include <stdexcept>

namespace ape
{

class Body;
class BodyPart;
class BodySelector;
class Camera;
class CameraSelector;
class Scene;
class ShapeRenderer;
class StandardShaderProgram;
class WireframeShaderProgram;

class SceneRenderer
{

public:

    SceneRenderer(
        CameraSelector const & cameraSelector,
        BodySelector const & pickedBodySelector,
        ShapeRenderer const & shapeRenderer,
        StandardShaderProgram & standardShader,
        WireframeShaderProgram & wireframeShader,
        glm::vec3 const & backgroundColor);

    auto render()
        -> void;

    auto getCameraSelector() const
        -> CameraSelector const &;

    auto setCameraSelector(CameraSelector const & newSelector)
        -> void;

    auto getOutliningStyle() const
        -> LineStyle;

    auto setOutliningStyle(LineStyle const & newStyle)
        -> void;

    auto getWireframeStyle() const
        -> LineStyle;

    auto setWireframeStyle(LineStyle const & newStyle)
        -> void;

private:

    auto hasActiveCamera() const
        -> bool;

    auto clear() const
        -> void;

    auto prepareBodyRenderers()
        -> void;

    auto renderBodies() const
        -> void;

    auto renderNonPickedBodies(Camera const & camera) const
        -> void;

    auto renderPickedBodies(Camera const & camera) const
        -> void;

private:

    CameraSelector const * cameraSelector;

    BodySelector const * pickedBodySelector;

    StandardBodyRenderer standardBodyRenderer;

    WireframeBodyRenderer wireframeBodyRenderer;

    OutlinedBodyRenderer outlinedBodyRenderer;

    LineStyle wireframeStyle;

    LineStyle outliningStyle;

    glm::vec3 backgroundColor;

    VertexArrayObject arrayObject;

};

auto getCamera(SceneRenderer const & renderer)
    -> Camera *;

auto getScene(SceneRenderer const & renderer)
    -> Scene &;

} // namespace ape
