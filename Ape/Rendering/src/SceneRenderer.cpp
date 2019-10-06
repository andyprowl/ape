#include <Rendering/SceneRenderer.hpp>

#include <Rendering/ShapeArrayObjectRenderer.hpp>
#include <Rendering/StandardShaderProgram.hpp>
#include <Rendering/WireframeShaderProgram.hpp>

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
    CameraSelector const & cameraSelector,
    BodySelector const & pickedBodySelector,
    ShapeRenderer const & shapeRenderer,
    StandardShaderProgram & standardShader,
    WireframeShaderProgram & wireframeShader,
    glm::vec3 const & backgroundColor)
    : cameraSelector{&cameraSelector}
    , pickedBodySelector{&pickedBodySelector}
    , standardBodyRenderer{standardShader, shapeRenderer}
    , wireframeBodyRenderer{wireframeShader, shapeRenderer}
    , outlinedBodyRenderer{standardBodyRenderer, wireframeBodyRenderer}
    , wireframeStyle{0.01f, {1.0f, 0.2f, 0.2f}}
    , outliningStyle{0.05f, {0.2f, 0.2f, 1.0f}}
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
    
    prepareBodyRenderers();

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

auto SceneRenderer::getOutliningStyle() const
    -> LineStyle
{
    return outliningStyle;
}

auto SceneRenderer::setOutliningStyle(LineStyle const & newStyle)
    -> void
{
    outliningStyle = newStyle;
}

auto SceneRenderer::getWireframeStyle() const
    -> LineStyle
{
    return wireframeStyle;
}

auto SceneRenderer::setWireframeStyle(LineStyle const & newStyle)
    -> void
{
    wireframeStyle = newStyle;
}

auto SceneRenderer::clear() const
    -> void
{
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

auto SceneRenderer::hasActiveCamera() const
    -> bool
{
    auto const activeCamera = cameraSelector->getActiveCamera();

    return (activeCamera != nullptr);
}

auto SceneRenderer::prepareBodyRenderers()
    -> void
{
    auto const activeCamera = cameraSelector->getActiveCamera();

    assert(activeCamera != nullptr);

    auto const & lighting = cameraSelector->getScene().getLighting();

    standardBodyRenderer.prepare(*activeCamera, lighting);
}

auto SceneRenderer::renderBodies() const
    -> void
{
    auto const arrayObjectBinder = ScopedBinder{arrayObject};

    auto const activeCamera = cameraSelector->getActiveCamera();

    assert(activeCamera != nullptr);

    renderNonPickedBodies(*activeCamera);

    renderPickedBodies(*activeCamera);
}

auto SceneRenderer::renderNonPickedBodies(Camera const & camera) const
    -> void
{
    auto const nonSelectedBodies = pickedBodySelector->getNonSelectedBodies();

    if (nonSelectedBodies.empty())
    {
        return;
    }

    standardBodyRenderer.render(nonSelectedBodies, camera);
}

auto SceneRenderer::renderPickedBodies(Camera const & camera) const
    -> void
{
    auto const selectedBodies = pickedBodySelector->getSelectedBodies();

    if (selectedBodies.empty())
    {
        return;
    }

    outlinedBodyRenderer.render(selectedBodies, camera, outliningStyle);
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
